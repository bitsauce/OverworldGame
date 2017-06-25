#include "ChunkManager.h"
#include "Game/Debug.h"
#include "World/World.h"
#include "Generation/Generator.h"
#include "BlockEntities/BlockEntityData.h"
#include "BlockEntities/BlockEntity.h"
#include "Entities/EntityData.h"
#include "Blocks/BlockData.h"
#include "Game/Game.h"

const float QUAD_UVS[40] =
{
	0,  16,
	8,  16,
	16, 16,
	24, 16,
	0,  24,
	8,  24,
	16, 24,
	24, 24,
	0,  32,
	8,  32,
	16, 32,
	24, 32,
	0,  40,
	8,  40,
	16, 40,
	24, 40,
	 0,  0,
	 8,  0,
	 0,  8,
	 8,  8
};

ChunkManager::ChunkManager(World *world, Window *window) :
	m_applyZoom(true),
	m_window(window),
	m_camera(world->getCamera()),
	m_generator(new ChunkGenerator(world->getSeed())),
	m_world(world),
	m_chunkPositionIndex(0),
	m_loadAreaRadius(5),
	m_circleLoadIndex(0),
	m_reattachAllChunks(true),
	m_time(0.0f)
{
	// Setup vertex format
	VertexFormat vertexFormat;
	vertexFormat.set(VERTEX_POSITION, 2);
	vertexFormat.set(VERTEX_TEX_COORD, 2);

	// Load tile map shaders
	m_tileSortShader = Resource<Shader>("Shaders/TileSort");
	m_tileSortShader->bindFragLocation(0, "out_BlockData");
	m_tileSortShader->bindFragLocation(1, "out_QuadData");
	m_tileSortShader->link();

	m_tileMapShader = Resource<Shader>("Shaders/TileMap");

	// Setup block entity shader
	m_blockEntityShader = Resource<Shader>("Shaders/BlockEntitiesDraw");
	m_blockEntityShader->setSampler2D("u_TextureAtlas", BlockEntityData::getTextureAtlas()->getTexture());
	m_blockEntityShader->setSampler2D("u_BlockEntityData", BlockEntityData::getDataTexture());

	// Setup block rendering shader
	m_tileMapShader->setSampler2D("u_BlockAtlas", BlockData::getTextureAtlas()->getTexture());
	m_tileMapShader->setSampler2D("u_BlockData", BlockData::getDataTexture());
	m_tileMapShader->setUniform2f("u_QuadUVs", QUAD_UVS);

	// Set chunk render caches to null (will be updated by updateViewSize)
	for(int i = 0; i < WORLD_LAYER_COUNT; ++i)
	{
		m_sortedBlocksRenderTarget[i] = 0;
	}
	m_blocksRenderTarget = 0;

	// Update size of textures and such
	updateViewSize(window->getWidth(), window->getHeight());
}

void ChunkManager::clear()
{
	while(!m_chunks.empty())
	{
		freeChunk(m_chunks.begin());
	}
}

// SERIALIZATION
void ChunkManager::saveBlockData(FileWriter &file, Block *blockData)
{
	// Write blocks to stream
	BlockID currentBlock = blockData[0].getBlockData()->getID();
	int length = 1;
	for(int i = 1; i < CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT; ++i)
	{
		BlockID block = blockData[i].getBlockData()->getID();
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

void ChunkManager::loadBlockData(FileReader &file, Block *blockData)
{
	// Read blocks from stream
	int pos = 0;
	while(pos < CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT)
	{
		int blockID, length;
		file >> blockID;
		file >> length;
		for(int i = 0; i < length; ++i)
		{
			blockData[pos + i].setBlockDataByID(blockID);
		}
		pos += length;
	}
}

void ChunkManager::saveEntities(FileWriter &file, list<Entity*> entities)
{
	// Write entity data
	file << (int) entities.size() << endl;
	for(Entity *entity : entities)
	{
		file << entity->getData()->getID() << endl;
		entity->onSaveData(file);
	}
}

void ChunkManager::loadEntities(FileReader &file)
{
	// Read entity data
	int numEntities;
	file >> numEntities;
	for(int i = 0; i < numEntities; ++i)
	{
		int id;
		file >> id;
		if(id == /*ENTITY_PLAYER*/1) continue;
		Entity *entity = EntityData::Create(id, Json::Value());
		entity->onLoadData(file);
	}
}

/*void ChunkManager::saveEntities(FileWriter &file, list<BlockEntity*> entities)
{
	// Write entity data
	file << (int) entities.size() << endl;
	for(BlockEntity *entity : entities)
	{
		file << entity->getData()->getID() << endl;
		file << entity->getX() << endl;
		file << entity->getY() << endl;
	}
}

void ChunkManager::loadEntities(FileReader &file)
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
		m_world->getTerrain()->createBlockEntityAt(x, y, (BlockEntityID) id);
		//BlockEntityData::get((BlockEntityID) id)->create(m_world, x, y);
	}
}*/

bool ChunkManager::freeInactiveChunk()
{
	// Find a chunk offscreen and add it to the pool
	for(unordered_map<uint, Chunk*>::iterator itr = m_chunks.begin(); itr != m_chunks.end(); ++itr)
	{
		int x = itr->second->getX(), y = itr->second->getY();
		if(x < m_loadingArea.x0 || x > m_loadingArea.x1 || y < m_loadingArea.y0 || y > m_loadingArea.y1)
		{
			freeChunk(itr);
			return true;
		}
	}

	// No offscreen chunk found, do nothing
	return false;
}

void ChunkManager::freeChunk(unordered_map<uint, Chunk*>::iterator itr)
{
	// Free it
	Chunk *chunk = itr->second;

	// Open file writer
	const string chunkFilePath = util::getAbsoluteFilePath(m_world->getWorldPath() + "/Chunks/" + util::intToStr(itr->first) + ".obj");
	FileWriter file(chunkFilePath);
	if(file.isOpen())
	{
		// Save chunk data
		saveBlockData(file, chunk->m_blocks);
		saveEntities(file, chunk->m_entities);
	}
	else
	{
		LOG("Unable to save block data to '%s' (reason: %s)", chunkFilePath.c_str(), strerror(errno));
	}

	chunk->unload();

	// Add chunk to chunk pool and remove from active chunks
	m_chunkPool.push_back(chunk);
	m_chunks.erase(itr->first);
}

void ChunkManager::setOptimalChunkCount(const uint optimalChunkCount)
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
		m_chunkPool[i] = new Chunk(this);
	}

	// Set optimal chunk count
	m_optimalChunkCount = optimalChunkCount;
}

Chunk *ChunkManager::getChunkAt(const int chunkX, const int chunkY, const bool loadChunk)
{
	uint key = CHUNK_KEY(chunkX, chunkY);
	if(m_chunks.find(key) == m_chunks.end())
	{
		if(loadChunk)
		{
			// Load block data for this chunk
			return loadChunkAt(chunkX, chunkY);
		}
		else
		{
			return 0;
		}
	}
	return m_chunks[key];
}

Chunk *ChunkManager::loadChunkAt(const int chunkX, const int chunkY)
{
	// Do we have any available chunks?
	if(m_chunkPool.empty())
	{
		// This makes sure there is an available chunk in the pool
		if(!freeInactiveChunk())
		{
			THROW("Unable to free inactive chunk");
		}
	}

	// Grab a chunk from the pool
	Chunk *chunk = m_chunkPool.back();
	m_chunkPool.pop_back();

	// Get chunk file path
	const uint key = CHUNK_KEY(chunkX, chunkY);
	const string chunkFilePath = util::getAbsoluteFilePath(m_world->getWorldPath() + "/Chunks/" + util::intToStr(key) + ".obj");

	// If the chunk file exists, open a file reader
	FileReader *file = 0;
	if(util::fileExists(chunkFilePath))
	{
		file = new FileReader(chunkFilePath);
		if(!file->isOpen())
		{
			LOG("Unable to load block data from '%s' (reason: %s)", chunkFilePath.c_str(), strerror(errno));
			file = 0;
		}
	}

	// Get block data
	Block blocks[CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT];
	if(file)
	{
		LOG("Loading chunk [%i, %i]...", chunkX, chunkY);

		// Load chunk from file
		loadBlockData(*file, blocks);
	}
	else
	{
		// Generate block data
		m_generator->getBlocks(chunkX, chunkY, blocks);
	}

	// Initialize chunk
	(m_chunks[key] = chunk)->load(chunkX, chunkY, blocks);

	// Load entities if any
	if(file)
	{
		loadEntities(*file);
		delete file;
	}

	return chunk;
}

bool ChunkManager::isChunkLoadedAt(const int chunkX, const int chunkY) const
{
	return m_chunks.find(CHUNK_KEY(chunkX, chunkY)) != m_chunks.end();
}

ChunkManager::ChunkArea ChunkManager::getLoadingArea() const
{
	return m_loadingArea;
}

ChunkManager::ChunkArea ChunkManager::getActiveArea() const
{
	return m_activeArea;
}

void ChunkManager::onTick(TickEvent *e)
{
	// Update block animation
	m_tileMapShader->setUniform1f("u_Time", m_time);
	m_time += e->getDelta();

	m_blockEntityShader->setUniform1f("u_Time", m_time);
}

void ChunkManager::onDraw(DrawEvent *e)
{
	// Update active chunk area
	Vector2F center = m_camera->getCenter(e->getAlpha());
	Vector2F size = m_applyZoom ? m_camera->getSize() : m_window->getSize();

	// Get active area
	m_activeArea.x0 = (int) floor(center.x / CHUNK_PXF) - (int) floor(size.x * 0.5f / CHUNK_PXF) - 1;
	m_activeArea.y0 = (int) floor(center.y / CHUNK_PXF) - (int) floor(size.y * 0.5f / CHUNK_PXF) - 1;
	m_activeArea.x1 = (int) floor(center.x / CHUNK_PXF) + (int) floor(size.x * 0.5f / CHUNK_PXF) + 1;
	m_activeArea.y1 = (int) floor(center.y / CHUNK_PXF) + (int) floor(size.y * 0.5f / CHUNK_PXF) + 1;

	// Get loading area
	m_loadingArea.x0 = m_activeArea.x0 - m_loadAreaRadius;
	m_loadingArea.y0 = m_activeArea.y0 - m_loadAreaRadius;
	m_loadingArea.x1 = m_activeArea.x1 + m_loadAreaRadius;
	m_loadingArea.y1 = m_activeArea.y1 + m_loadAreaRadius;

	// Free inactive chunks
	if(m_chunks.size() >= m_optimalChunkCount)
	{
		freeInactiveChunk();
	}

	// Setup context
	GraphicsContext *context = e->getGraphicsContext();
	context->disable(GraphicsContext::BLEND);
	context->setTransformationMatrix(Matrix4());

	// This should be called when a new area is entered
	if(m_reattachAllChunks)
	{
		// Detach chunks
		for(int y = m_loadingArea.y0 + 1; y <= m_loadingArea.y1 - 1; ++y)
		{
			for(int x = m_loadingArea.x0 + 1; x <= m_loadingArea.x1 - 1; ++x)
			{
				getChunkAt(x, y, true)->detach();
			}
		}

		// Attach chunks
		for(int y = m_loadingArea.y0 + 1; y <= m_loadingArea.y1 - 1; ++y)
		{
			for(int x = m_loadingArea.x0 + 1; x <= m_loadingArea.x1 - 1; ++x)
			{
				reattachChunk(getChunkAt(x, y, true), context);
			}
		}

		// Set state variables
		m_prevLoadingArea = m_loadingArea;
		m_reattachAllChunks = false;
	}
	else if(m_loadingArea != m_prevLoadingArea)
	{
		// Get chunks left
		int x0, x1, y0, y1;
		if(m_loadingArea.x0 > m_prevLoadingArea.x0) // Moved right
		{
			x0 = m_prevLoadingArea.x0;
			x1 = min(m_prevLoadingArea.x1, m_loadingArea.x0) - 1;
		}
		else if(m_loadingArea.x0 < m_prevLoadingArea.x0) // Moved left
		{
			x0 = max(m_prevLoadingArea.x0, m_loadingArea.x1) + 1;
			x1 = m_prevLoadingArea.x1;
		}
		else
		{
			x0 = m_prevLoadingArea.x0;
			x1 = m_prevLoadingArea.x1;
		}

		if(m_loadingArea.y0 > m_prevLoadingArea.y0) // Moved down
		{
			y0 = m_prevLoadingArea.y0;
			y1 = min(m_prevLoadingArea.y1, m_loadingArea.y0) - 1;
		}
		else if(m_loadingArea.y0 < m_prevLoadingArea.y0) // Moved up
		{
			y0 = max(m_prevLoadingArea.y0, m_loadingArea.y1) + 1;
			y1 = m_prevLoadingArea.y1;
		}
		else
		{
			y0 = m_prevLoadingArea.y0;
			y1 = m_prevLoadingArea.y1;
		}

		// Detach chunks
		for(int x = x0; x <= x1; ++x)
		{
			for(int y = y0; y <= y1; ++y)
			{
				getChunkAt(x, y, true)->detach();
			}
		}

		if(m_prevLoadingArea.x0 != m_loadingArea.x0 && m_prevLoadingArea.y0 != m_loadingArea.y0)
		{
			for(int x = m_loadingArea.x0; x <= m_loadingArea.x1; ++x)
			{
				for(int y = y0; y <= y1; ++y)
				{
					getChunkAt(x, y, true)->detach();
				}
			}

			for(int x = x0; x <= x1; ++x)
			{
				for(int y = m_loadingArea.y0; y <= m_loadingArea.y1; ++y)
				{
					getChunkAt(x, y, true)->detach();
				}
			}
		}

		// Store previous position
		m_chunkPositions[m_chunkPositionIndex++ % 4] = Vector2I(m_loadingArea.x0, m_loadingArea.x1);
		m_circleLoadIndex = 0; // Reset iterator

		// Store previous position
		m_prevLoadingArea = m_loadingArea;
	}

	// Make sure all chunks in the active area are attached
	for(int y = m_activeArea.y0; y <= m_activeArea.y1; ++y)
	{
		for(int x = m_activeArea.x0; x <= m_activeArea.x1; ++x)
		{
			// Is this chunk not attached?
			Chunk *chunk = getChunkAt(x, y, true);
			if(!chunk->isAttached() || !chunk->isSorted())
			{
				// Attach chunk
				reattachChunk(chunk, context);
			}
		}
	}

	// Load chunks in the loading area using a sprial loading pattern
	if(m_circleLoadPattern.size() > 0)
	{
		// Loop through 10 chunks to find one which is not attached
		for(int i = 0; i < 10; ++i)
		{
			// Get next chunk in the circular load pattern
			Vector2I centerChunkPosition((int) floor(center.x / CHUNK_PXF), (int) floor(center.y / CHUNK_PXF));
			Chunk *chunk = getChunkAt(centerChunkPosition.x + m_circleLoadPattern[m_circleLoadIndex].x, centerChunkPosition.y + m_circleLoadPattern[m_circleLoadIndex].y, true);

			// Increase load pattern index by 1
			m_circleLoadIndex = (m_circleLoadIndex + 1) % m_circleLoadPattern.size();

			// Is this chunk not attached?
			if(!chunk->isAttached() || !chunk->isSorted())
			{
				// Attach chunk
				reattachChunk(chunk, context);

				// Found a chunk which was not attached, break out
				break;
			}
		}
	}

	context->setRenderTarget(0);
	context->enable(GraphicsContext::BLEND);
}

void ChunkManager::onWindowSizeChanged(WindowEvent *e)
{
	updateViewSize(e->getWidth(), e->getHeight());
}

struct VectorComparator
{
	bool operator() (const Vector2I &v0, const Vector2I &v1)
	{
		return v0.magnitude() > v1.magnitude();
	}
};

void ChunkManager::updateViewSize(int width, int height)
{
	// Calculate load area size
	int loadAreaWidth = (int) (floor(width * 0.5f / CHUNK_PXF) * 2 + 3) + m_loadAreaRadius * 2;
	int loadAreaHeight = (int) (floor(height * 0.5f / CHUNK_PXF) * 2 + 3) + m_loadAreaRadius * 2;

	// Set optimal chunk count
	setOptimalChunkCount(loadAreaWidth * loadAreaHeight * 2); // 15x13

	// Create circle load pattern
	priority_queue<Vector2I, vector<Vector2I>, VectorComparator> minHeap;
	for(int y = (int) -floor((loadAreaHeight - 2) * 0.5f); y <= (int) floor((loadAreaHeight - 2) * 0.5f); ++y)
	{
		for(int x = (int) -floor((loadAreaWidth - 2) * 0.5f); x <= (int) floor((loadAreaWidth - 2) * 0.5f); ++x)
		{
			minHeap.push(Vector2I(x, y));
		}
	}

	while(!minHeap.empty())
	{
		m_circleLoadPattern.push_back(minHeap.top());
		minHeap.pop();
	}

	// Create new render targets
	for(int i = 0; i < WORLD_LAYER_COUNT; ++i)
	{
		delete m_sortedBlocksRenderTarget[i];
		m_sortedBlocksRenderTarget[i] = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS, 2, PixelFormat(PixelFormat::RGBA, PixelFormat::UNSIGNED_INT));
		m_sortedBlocksRenderTarget[i]->getTexture(0)->setWrapping(Texture2D::REPEAT);
		m_sortedBlocksRenderTarget[i]->getTexture(1)->setWrapping(Texture2D::REPEAT);
	}

	delete m_blocksRenderTarget;
	m_blocksRenderTarget = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_blocksRenderTarget->getTexture()->setWrapping(Texture2D::REPEAT);
	m_blocksRenderTarget->getTexture()->setFiltering(Texture2D::NEAREST);

	// Set shader uniforms
	m_tileSortShader->setSampler2D("u_BlockGrid", m_blocksRenderTarget->getTexture());
	
	// Redraw blocks
	m_reattachAllChunks = true;
}

void ChunkManager::reattachChunk(Chunk *chunk, GraphicsContext *context)
{
	// TODO: Not sure if this is right...
	m_world->getLighting()->m_redrawStaticLighting = true;

	// Attach this chunk's and its neighbours' block data to the block data render-target
	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			Chunk *neighbour = getChunkAt(chunk->getX() + i, chunk->getY() + j, true);
			if(!neighbour->isAttached())
			{
				//context->disable(GraphicsContext::BLEND);
				context->setRenderTarget(m_blocksRenderTarget);
				neighbour->attach(context, math::mod(neighbour->getX(), m_loadingArea.getWidth()), math::mod(neighbour->getY(), m_loadingArea.getHeight()));
			}
		}
	}
	
	// Sort blocks
	if(!chunk->isSorted())
	{
		context->setShader(m_tileSortShader);
		for(int z = 0; z < WORLD_LAYER_COUNT; ++z)
		{
			m_tileSortShader->setUniform1ui("u_Layer", z);
			context->setRenderTarget(m_sortedBlocksRenderTarget[z]);

			TextureRegion textureRegion(
				float(chunk->getX()) / float(m_loadingArea.getWidth()),
				float(chunk->getY()) / float(m_loadingArea.getHeight()),
				float(chunk->getX() + 1) / float(m_loadingArea.getWidth()),
				float(chunk->getY() + 1) / float(m_loadingArea.getHeight())
				);

			context->drawRectangle(
				math::mod(chunk->getX(), m_loadingArea.getWidth())  * CHUNK_BLOCKSF,
				math::mod(chunk->getY(), m_loadingArea.getHeight()) * CHUNK_BLOCKSF,
				CHUNK_BLOCKSF, CHUNK_BLOCKSF, Color::White, textureRegion
				);
		}
		chunk->m_sorted = true;
	}
	context->setShader(0);
	context->setRenderTarget(0);
}