#include "ChunkLoader.h"
#include "Game/Debug.h"
#include "World/Camera.h"
#include "World/World.h"
#include "Generation/Generator.h"
#include "Entities/Static/StaticEntity.h"
#include "Entities/EntityData.h"
#include "Blocks/BlockData.h"

#define CHUNK_KEY(X, Y) ((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000)

const float QUAD_UVS[40] =
{
	1, 25,
	9, 25,
	17, 25,
	25, 25,
	1, 17,
	9, 17,
	17, 17,
	25, 17,
	1, 9,
	9, 9,
	17, 9,
	25, 9,
	1, 1,
	9, 1,
	17, 1,
	25, 1,
	1, 41,
	9, 41,
	1, 33,
	9, 33
};

ChunkLoader::ChunkLoader(World *world) :
	m_applyZoom(true),
	m_camera(world->getCamera()),
	m_generator(world->getGenerator()),
	m_world(world),
	m_prevChunkPosition(0, 0),
	m_chunkPositionIndex(0),
	m_loadAreaRadius(5),
	m_circleLoadIndex(0),
	m_redrawGlobalBlocks(true)
{
	// Setup vertex format
	VertexFormat vertexFormat;
	vertexFormat.set(VERTEX_POSITION, 2);
	vertexFormat.set(VERTEX_TEX_COORD, 2);

	// Load tile map shaders
	m_tileSortShader = ResourceManager::get<Shader>(":/Shaders/TileSort");
	m_tileSortShader->bindFragLocation(0, "out_BlockData");
	m_tileSortShader->bindFragLocation(1, "out_QuadData");
	m_tileSortShader->link();

	m_tileMapShader = ResourceManager::get<Shader>(":/Shaders/TileMap");
	//m_tileSortShader->exportAssembly(":/Shaders/TileSort.bin");
	//m_tileMapShader->exportAssembly(":/Shaders/TileMap.bin");

	// Set block atlas
	m_tileMapShader->setSampler2D("u_BlockAtlas", BlockData::getBlockAtlas()->getTexture());
	m_tileMapShader->setUniform2f("u_QuadUVs", QUAD_UVS);

	// Set max chunks to some value
	setOptimalChunkCount(0);

	// Set chunk render caches to null (will be updated later)
	for(int i = 0; i < TERRAIN_LAYER_COUNT; ++i)
	{
		m_sortedBlocksRenderTarget[i] = 0;
	}
	m_blocksRenderTarget = 0;
}

void ChunkLoader::clear()
{
	while(!m_chunks.empty())
	{
		freeChunk(m_chunks.begin());
	}
}

// SERIALIZATION
void ChunkLoader::saveBlockData(FileWriter &file, BlockID * blockData)
{
	// Write blocks to stream
	BlockID currentBlock = blockData[0];
	int length = 1;
	for(int i = 1; i < CHUNK_BLOCKS * CHUNK_BLOCKS * TERRAIN_LAYER_COUNT; ++i)
	{
		BlockID block = blockData[i];
		if(block == currentBlock)
		{
			length++;
		}
		else
		{
			file << currentBlock << endl;
			file << length << endl;
			length = 1;
			currentBlock = block;
		}
	}
	file << currentBlock << endl;
	file << length << endl;
}

void ChunkLoader::loadBlockData(FileReader &file, BlockID *blockData)
{
	// Read blocks from stream
	int pos = 0;
	while(pos < CHUNK_BLOCKS * CHUNK_BLOCKS * TERRAIN_LAYER_COUNT)
	{
		int block, length;
		file >> block;
		file >> length;
		for(int i = 0; i < length; ++i)
		{
			blockData[pos + i] = (BlockID)block;
		}
		pos += length;
	}
}

void ChunkLoader::saveEntities(FileWriter &file, set<StaticEntity*> entities)
{
	// Write entity data
	file << (int) entities.size() << endl;
	for(StaticEntity * entity : entities)
	{
		file << entity->getID() << endl;
		file << entity->getX() << endl;
		file << entity->getY() << endl;
		entity->createSaveData(file);
	}
}

void ChunkLoader::loadEntities(FileReader &file)
{
	// Read entity data
	int numEntities;
	file >> numEntities;
	for(int i = 0; i < numEntities; ++i)
	{
		int id, x, y;
		file >> id;
		file >> x;
		file >> y;
		((StaticEntityData*) EntityData::Get((EntityID) id))->Create(m_world, x, y)->loadSaveData(file);
	}
}

bool ChunkLoader::freeInactiveChunk()
{
	// Find a chunk offscreen and add it to the pool
	for(unordered_map<uint, Chunk*>::iterator itr = m_chunks.begin(); itr != m_chunks.end(); ++itr)
	{
		int x = itr->second->getX(), y = itr->second->getY();
		if(x < m_loadArea.x0 || x > m_loadArea.x1 || y < m_loadArea.y0 || y > m_loadArea.y1)
		{
			freeChunk(itr);
			return true;
		}
	}

	// No offscreen chunk found, do nothing
	return false;
}

void ChunkLoader::freeChunk(unordered_map<uint, Chunk*>::iterator itr)
{
	// Free it
	Chunk *chunk = itr->second;

	// Open file writer
	string filePath = util::getAbsoluteFilePath(m_world->getWorldPath() + "/Chunks/" + util::intToStr(itr->first) + ".obj");
	FileWriter file(filePath);
	if(!file.isOpen())
	{
		LOG("Unable to load block data from '%s'", filePath);
		return;
	}

	// Save chunk data
	saveBlockData(file, chunk->m_blocks);
	saveEntities(file, chunk->m_staticEntitites);
	
	// Delete static entities
	for(StaticEntity *entity : chunk->m_staticEntitites)
	{
		delete entity;
	}
	chunk->m_staticEntitites.clear(); // Empty static entity list

	// Add chunk to chunk pool and remove from active chunks
	m_chunkPool.push_back(chunk);
	m_chunks.erase(itr->first);
}

void ChunkLoader::setOptimalChunkCount(const uint optimalChunkCount)
{
	// Clear old chunks
	for(uint i = 0; i < m_chunkPool.size(); ++i)
	{
		delete m_chunkPool[i];
	}

	// Create new chunks
	m_chunkPool.resize(uint(optimalChunkCount * 1.1f));
	for(uint i = 0; i < m_chunkPool.size(); ++i)
	{
		m_chunkPool[i] = new Chunk(m_tileMapShader, m_tileSortShader);
	}

	// Set optimal chunk count
	m_optimalChunkCount = optimalChunkCount;
}

void ChunkLoader::loadActiveArea()
{
	for(int y = m_activeArea.y0-1; y <= m_activeArea.y1+1; y++)
	{
		for(int x = m_activeArea.x0-1; x <= m_activeArea.x1+1; x++)
		{
			Chunk &chunk = getChunkAt(x, y);
			chunk.updateTileMap(this);
		}
	}
}

Chunk &ChunkLoader::getChunkAt(const int chunkX, const int chunkY)
{
	uint key = CHUNK_KEY(chunkX, chunkY);
	if(m_chunks.find(key) == m_chunks.end())
	{
		// Load block data for this chunk
		return *loadChunkAt(chunkX, chunkY);
	}
	return *m_chunks[key];
}

Chunk *ChunkLoader::loadChunkAt(const int chunkX, const int chunkY)
{
	if(m_chunkPool.empty()) {
		assert(freeInactiveChunk()); // This makes sure there is an available chunk in the pool
	}
	
	// Grab a chunk from the pool
	Chunk *chunk = m_chunkPool.back();
	m_chunkPool.pop_back();

	// Get chunk file path
	uint key = CHUNK_KEY(chunkX, chunkY);
	string chunkFilePath = util::getAbsoluteFilePath(m_world->getWorldPath() + "/Chunks/" + util::intToStr(key) + ".obj");

	// Open file writer
	FileReader *file = 0;
	if(util::fileExists(chunkFilePath))
	{
		file = new FileReader(chunkFilePath);
		if(!file->isOpen())
		{
			LOG("Unable to save block data to '%s'", chunkFilePath);
			//return;
			file = 0;
		}
	}

	// Get block data
	BlockID blocks[CHUNK_BLOCKS * CHUNK_BLOCKS * TERRAIN_LAYER_COUNT];
	if(file)
	{
		LOG("Loading chunk [%i, %i]...", chunkX, chunkY);

		// Load chunk from file
		loadBlockData(*file, blocks);
	}
	else
	{
		// Generate block data
		m_generator->getChunkBlocks(chunkX, chunkY, blocks);
	}

	// Initialize chunk
	(m_chunks[key] = chunk)->load(chunkX, chunkY, blocks);

	// Load entities if any
	if(file)
	{
		loadEntities(*file);
	}

	return chunk;
}

bool ChunkLoader::isChunkLoadedAt(const int chunkX, const int chunkY) const
{
	return m_chunks.find(CHUNK_KEY(chunkX, chunkY)) != m_chunks.end();
}

ChunkLoader::ChunkArea ChunkLoader::getActiveArea() const
{
	return m_activeArea;
}

ChunkLoader::ChunkArea ChunkLoader::getLoadArea() const
{
	return m_loadArea;
}

void ChunkLoader::update()
{
}

void ChunkLoader::draw(GraphicsContext &context, const float alpha)
{
	// Update active chunk area
	Vector2 center = m_camera->getCenter(alpha);
	Vector2 size = m_applyZoom ? m_camera->getSize() : Window::getSize();

	// Active area should have the same center as the view
	m_activeArea.x0 = (int) floor(center.x / CHUNK_PXF) - (int) floor(size.x * 0.5f / CHUNK_PXF) - 1;
	m_activeArea.y0 = (int) floor(center.y / CHUNK_PXF) - (int) floor(size.y * 0.5f / CHUNK_PXF) - 1;
	m_activeArea.x1 = (int) floor(center.x / CHUNK_PXF) + (int) floor(size.x * 0.5f / CHUNK_PXF) + 1;
	m_activeArea.y1 = (int) floor(center.y / CHUNK_PXF) + (int) floor(size.y * 0.5f / CHUNK_PXF) + 1;

	// Update load area
	m_loadArea.x0 = m_activeArea.x0 - m_loadAreaRadius;
	m_loadArea.y0 = m_activeArea.y0 - m_loadAreaRadius;
	m_loadArea.x1 = m_activeArea.x1 + m_loadAreaRadius;
	m_loadArea.y1 = m_activeArea.y1 + m_loadAreaRadius;

	// Check if new chunks were entered
	Vector2i chunkPosition(m_activeArea.x0, m_activeArea.y0);
	if(chunkPosition != m_prevChunkPosition)
	{
		// Calculate average position
		m_averagePosition.set(0.0f, 0.0f);
		for(int i = 0; i < 4; ++i)
		{
			m_averagePosition += m_chunkPositions[i];
		}
		m_averagePosition /= 4.0f;

		// Store previous position
		m_prevChunkPosition = m_chunkPositions[m_chunkPositionIndex++ % 4] = chunkPosition;
		m_circleLoadIndex = 0; // Reset iterator
	}

	// Free inactive chunks
	if(m_chunks.size() >= m_optimalChunkCount)
	{
		freeInactiveChunk();
	}

	// Load offscren chunks
	if(m_circleLoadPattern.size() > 0)
	{
		Vector2i centerChunkPosition((int) floor(center.x / CHUNK_PXF), (int) floor(center.y / CHUNK_PXF));
		Chunk &chunk = getChunkAt(centerChunkPosition.x + m_circleLoadPattern[m_circleLoadIndex].x, centerChunkPosition.y + m_circleLoadPattern[m_circleLoadIndex].y);
		if(chunk.isDirty()) chunk.updateTileMap(this);
		m_circleLoadIndex = (m_circleLoadIndex + 1) % m_circleLoadPattern.size();
	}

	if(m_redrawGlobalBlocks || Input::getKeyState(XD_KEY_0))
	{
		// Render all caches
		context.disable(GraphicsContext::BLEND);
		context.setRenderTarget(m_blocksRenderTarget);
		for(int y = m_activeArea.y0; y <= m_activeArea.y1; ++y)
		{
			for(int x = m_activeArea.x0; x <= m_activeArea.x1; ++x)
			{
				Matrix4 mat;
				mat.translate((x - m_activeArea.x0) * CHUNK_BLOCKS, (y - m_activeArea.y0) * CHUNK_BLOCKS, 0.0f);
				context.setModelViewMatrix(mat);
				getChunkAt(x, y).drawBlocks(context, this);
			}
		}

		Pixmap pixmap = m_blocksRenderTarget->getTexture()->getPixmap();

		context.setShader(m_tileSortShader);
		context.setModelViewMatrix(Matrix4());
		for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
		{
			m_tileSortShader->setUniform1ui("u_Layer", z);
			context.setRenderTarget(m_sortedBlocksRenderTarget[z]);
			context.drawRectangle(0, 0, (m_activeArea.x1 - m_activeArea.x0 + 1) * CHUNK_BLOCKS, (m_activeArea.y1 - m_activeArea.y0 + 1) * CHUNK_BLOCKS);
		}
		context.setRenderTarget(0);
		context.setShader(0);

		context.enable(GraphicsContext::BLEND);

		m_prevActiveArea = m_activeArea;
		m_redrawGlobalBlocks = false;
		m_globalChunkPosition = Vector2i(0, 0);
	}
	else if(m_activeArea != m_prevActiveArea)
	{
		// TODO: There appears to still be a bug when moving in both x and y direction at the same time.
		// Update new chunks
		int dx = m_activeArea.x0 - m_prevActiveArea.x0;
		int dy = m_activeArea.y0 - m_prevActiveArea.y0;

		// Draw new chunks into global block render target
		context.disable(GraphicsContext::BLEND);
		context.setRenderTarget(m_blocksRenderTarget);
		if(dx > 0)
		{
			for(int x = 0; x < dx; ++x)
			{
				for(int y = 0; y < m_activeArea.getHeight(); ++y)
				{
					Matrix4 mat;
					mat.translate(math::mod(m_globalChunkPosition.x + x, m_activeArea.getWidth()) * CHUNK_BLOCKS, math::mod(m_globalChunkPosition.y + y, m_activeArea.getHeight()) * CHUNK_BLOCKS, 0.0f);
					context.setModelViewMatrix(mat);
					getChunkAt(m_prevActiveArea.x1 + x + 1, m_activeArea.y0 + y).drawBlocks(context, this);
				}
			}
		}
		else if(dx < 0)
		{
			for(int x = 0; x < -dx; ++x)
			{
				for(int y = 0; y < m_activeArea.getHeight(); ++y)
				{
					Matrix4 mat;
					mat.translate(math::mod(m_globalChunkPosition.x - x - 1, m_activeArea.getWidth()) * CHUNK_BLOCKS, math::mod(m_globalChunkPosition.y + y, m_activeArea.getHeight()) * CHUNK_BLOCKS, 0.0f);
					context.setModelViewMatrix(mat);
					getChunkAt(m_prevActiveArea.x0 - x - 1, m_activeArea.y0 + y).drawBlocks(context, this);
				}
			}
		}
		m_globalChunkPosition.x += dx;
		
		if(dy > 0)
		{
			for(int y = 0; y < dy; ++y)
			{
				for(int x = 0; x < m_activeArea.getWidth(); ++x)
				{
					Matrix4 mat;
					mat.translate(math::mod(m_globalChunkPosition.x + x, m_activeArea.getWidth()) * CHUNK_BLOCKS, math::mod(m_globalChunkPosition.y + y, m_activeArea.getHeight()) * CHUNK_BLOCKS, 0.0f);
					context.setModelViewMatrix(mat);
					getChunkAt(m_activeArea.x0 + x, m_prevActiveArea.y1 + y + 1).drawBlocks(context, this);
				}
			}
		}
		else if(dy < 0)
		{
			for(int y = 0; y < -dy; ++y)
			{
				for(int x = 0; x < m_activeArea.getWidth(); ++x)
				{
					Matrix4 mat;
					mat.translate(math::mod(m_globalChunkPosition.x + x, m_activeArea.getWidth()) * CHUNK_BLOCKS, math::mod(m_globalChunkPosition.y - y - 1, m_activeArea.getHeight()) * CHUNK_BLOCKS, 0.0f);
					context.setModelViewMatrix(mat);
					getChunkAt(m_activeArea.x0 + x, m_prevActiveArea.y0 - y - 1).drawBlocks(context, this);
				}
			}
		}
		m_globalChunkPosition.y += dy;

		// Sort global block render target
		context.setShader(m_tileSortShader);
		context.setModelViewMatrix(Matrix4());
		for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
		{
			m_tileSortShader->setUniform1ui("u_Layer", z);
			context.setRenderTarget(m_sortedBlocksRenderTarget[z]);
			context.drawRectangle(0, 0, (m_activeArea.x1 - m_activeArea.x0 + 1) * CHUNK_BLOCKS, (m_activeArea.y1 - m_activeArea.y0 + 1) * CHUNK_BLOCKS);
		}

		// Reset graphics context
		context.setRenderTarget(0);
		context.setShader(0);
		context.enable(GraphicsContext::BLEND);

		// Store previous position
		m_prevActiveArea = m_activeArea;
	}
}

struct VectorComparator
{
	bool operator() (const Vector2i &v0, const Vector2i &v1)
	{
		return v0.magnitude() > v1.magnitude();
	}
};

void ChunkLoader::resizeEvent(uint width, uint height)
{
	int activeAreaWidth = (int) (floor(width  * 0.5f / CHUNK_PXF) * 2 + 3);
	int activeAreaHeight = (int) (floor(height * 0.5f / CHUNK_PXF) * 2 + 3);
	int loadAreaWidth  = activeAreaWidth + m_loadAreaRadius * 2;
	int loadAreaHeight = activeAreaHeight + m_loadAreaRadius * 2;

	setOptimalChunkCount(loadAreaWidth * loadAreaHeight * 2);
	
	priority_queue<Vector2i, vector<Vector2i>, VectorComparator> minHeap;
	for(int y = (int) -floor(loadAreaHeight*0.5f) + 1; y < (int) floor(loadAreaHeight*0.5f); ++y)
	{
		for(int x = (int) -floor(loadAreaWidth*0.5f) + 1; x < (int) floor(loadAreaWidth*0.5f); ++x)
		{
			minHeap.push(Vector2i(x, y));
		}
	}

	while(!minHeap.empty())
	{
		m_circleLoadPattern.push_back(minHeap.top());
		minHeap.pop();
	}

	for(int i = 0; i < TERRAIN_LAYER_COUNT; ++i)
	{
		delete m_sortedBlocksRenderTarget[i];
		m_sortedBlocksRenderTarget[i] = new RenderTarget2D(activeAreaWidth * CHUNK_BLOCKS, activeAreaHeight * CHUNK_BLOCKS, 2, PixelFormat(PixelFormat::RGBA, PixelFormat::UNSIGNED_INT));
		m_sortedBlocksRenderTarget[i]->getTexture(0)->setWrapping(Texture2D::REPEAT);
		m_sortedBlocksRenderTarget[i]->getTexture(1)->setWrapping(Texture2D::REPEAT);
	}

	delete m_blocksRenderTarget;
	m_blocksRenderTarget = new RenderTarget2D(activeAreaWidth * CHUNK_BLOCKS, activeAreaHeight * CHUNK_BLOCKS);

	m_tileSortShader->setSampler2D("u_BlockGrid", m_blocksRenderTarget->getTexture());
}