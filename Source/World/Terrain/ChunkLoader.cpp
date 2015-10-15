#include "ChunkLoader.h"
#include "Game/Debug.h"
#include "World/Camera.h"
#include "World/World.h"
#include "Generation/Generator.h"

#define CHUNK_KEY(X, Y) ((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000)

ChunkLoader::ChunkLoader(World *world) :
	m_applyZoom(true),
	m_camera(world->getCamera()),
	m_generator(world->getGenerator()),
	m_world(world),
	m_chunkPositionIndex(0),
	m_loadAreaRadius(5),
	m_circleLoadIndex(0)
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
	m_tileSortShader->exportAssembly(":/Shaders/TileSort.bin");
	m_tileMapShader->exportAssembly(":/Shaders/TileMap.bin");

	// Set max chunks to some value
	setOptimalChunkCount(512);
}

void ChunkLoader::clear()
{
	while(!m_chunks.empty())
	{
		freeChunk(m_chunks.begin());
	}
}

// SERIALIZATION
void ChunkLoader::saveBlockData(const string &filePath, BlockID *blockData)
{
	// Open file writer
	FileWriter file(filePath);
	if(!file.isOpen())
	{
		LOG("Unable to save block data to '%s'", filePath);
		return;
	}

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
	
void ChunkLoader::loadBlockData(const string &filePath, BlockID *blockData)
{
	// Open file reader
	FileReader file(filePath);
	if(!file.isOpen())
	{
		LOG("Unable to load block data from '%s'", filePath);
		return;
	}

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
	//if(chunk->isModified())
	{
		saveBlockData(util::getAbsoluteFilePath(m_world->getWorldPath() + "/Chunks/" + util::intToStr(itr->first) + ".obj"), chunk->m_blocks);
	}
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
			chunk.updateTileMap(this, TERRAIN_LAYER_BACK);
			chunk.updateTileMap(this, TERRAIN_LAYER_MIDDLE);
			chunk.updateTileMap(this, TERRAIN_LAYER_FRONT);
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
	string chunkFilePath = m_world->getWorldPath() + "/Chunks/" + util::intToStr(key) + ".obj";

	// Get block data
	BlockID blocks[CHUNK_BLOCKS*CHUNK_BLOCKS*TERRAIN_LAYER_COUNT];
	if(util::fileExists(chunkFilePath))
	{
		LOG("Loading chunk [%i, %i]...", chunkX, chunkY);

		// Load chunk from file
		loadBlockData(util::getAbsoluteFilePath(chunkFilePath), blocks);
	}
	else
	{
		// Generate block data
		m_generator->getChunkBlocks(chunkX, chunkY, blocks);
	}

	// Initialize chunk
	(m_chunks[key] = chunk)->load(chunkX, chunkY, blocks);
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

/*set<Thing*> ChunkLoader::getActiveThings()
{
	set<Thing*> things;
	for(int y = m_activeArea.y0-1; y <= m_activeArea.y1+1; y++)
	{
		for(int x = m_activeArea.x0-1; x <= m_activeArea.x1+1; x++)
		{
			for(Thing *thing : getChunkAt(x, y).getThings())
			{
				things.insert(thing);
			}
		}
	}
	return things;
}*/

void ChunkLoader::update()
{
	// Update active chunk area
	Vector2 center = m_camera->getCenter();
	Vector2 size = m_applyZoom ? m_camera->getSize() : Window::getSize();

	// Active area should have the same center as the view
	m_activeArea.x0 = (int)floor(center.x/CHUNK_PXF) - (int)floor(size.x*0.5f/CHUNK_PXF) - 1;
	m_activeArea.y0 = (int)floor(center.y/CHUNK_PXF) - (int)floor(size.y*0.5f/CHUNK_PXF) - 1;
	m_activeArea.x1 = (int)floor(center.x/CHUNK_PXF) + (int)floor(size.x*0.5f/CHUNK_PXF) + 1;
	m_activeArea.y1 = (int)floor(center.y/CHUNK_PXF) + (int)floor(size.y*0.5f/CHUNK_PXF) + 1;

	// Update load area
	m_loadArea.x0 = m_activeArea.x0 - m_loadAreaRadius;
	m_loadArea.y0 = m_activeArea.y0 - m_loadAreaRadius;
	m_loadArea.x1 = m_activeArea.x1 + m_loadAreaRadius;
	m_loadArea.y1 = m_activeArea.y1 + m_loadAreaRadius;
	
	// Calculate where the view is heading
	Vector2i chunkPosition(m_activeArea.x0, m_activeArea.y0);
	if(!(chunkPosition == m_prevChunkPosition))
	{
		// Calculate averate position
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

	Vector2 dir = Vector2(chunkPosition) - m_averagePosition;
	//World::getDebug()->setVariable("Chunk load direction", "[" + util::floatToStr(dir.x) + ", " + util::floatToStr(dir.y) + "]");

	if(m_chunks.size() >= m_optimalChunkCount)
	{
		// Free inactive chunks
		freeInactiveChunk();
	}

	// Load offscren chunks
	if(m_circleLoadPattern.size() > 0)
	{
		Vector2i centerChunkPosition((int) floor(center.x / CHUNK_PXF), (int) floor(center.y / CHUNK_PXF));
		Chunk &chunk = getChunkAt(centerChunkPosition.x + m_circleLoadPattern[m_circleLoadIndex].x, centerChunkPosition.y + m_circleLoadPattern[m_circleLoadIndex].y);
		if(chunk.isDirty(TERRAIN_LAYER_BACK)) chunk.updateTileMap(this, TERRAIN_LAYER_BACK);
		if(chunk.isDirty(TERRAIN_LAYER_MIDDLE)) chunk.updateTileMap(this, TERRAIN_LAYER_MIDDLE);
		if(chunk.isDirty(TERRAIN_LAYER_FRONT)) chunk.updateTileMap(this, TERRAIN_LAYER_FRONT);
		m_circleLoadIndex = (m_circleLoadIndex + 1) % m_circleLoadPattern.size();
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
	int loadAreaWidth  = (int)(floor(width  * 0.5f / CHUNK_PXF) * 2 + 3) + m_loadAreaRadius * 2;
	int loadAreaHeight = (int)(floor(height * 0.5f / CHUNK_PXF) * 2 + 3) + m_loadAreaRadius * 2;

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
}