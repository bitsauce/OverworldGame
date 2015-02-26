#include "ChunkLoader.h"
#include "Game/Debug.h"
#include "Entities/Camera.h"
#include "World/World.h"
#include "Generation/Generator.h"

#define CHUNK_KEY(X, Y) ((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000)

ChunkLoader::ChunkLoader(Camera *camera, WorldGenerator *generator) :
	GameObject(DRAW_ORDER_CHUNK_LOADER),
	m_applyZoom(true),
	m_camera(camera),
	m_generator(generator),
	m_chunkPositionIndex(0),
	m_loadAreaRadius(5),
	m_circleLoadIndex(0)
{
	// Setup vertex format
	VertexFormat vertexFormat;
	vertexFormat.set(VERTEX_POSITION, 2);
	vertexFormat.set(VERTEX_TEX_COORD, 2);
	Chunk::s_vertices = vertexFormat.createVertices(4*12*16*16*3);

	// Set max chunks to some value
	setOptimalChunkCount(512);

	// Add window listener
	Window::addWindowListener(this);
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
	for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
	{
		for(int y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for(int x = 0; x < CHUNK_BLOCKS; ++x)
			{
				file << blockData[BLOCK_INDEX(x, y, (TerrainLayer)z)] << endl;
			}
		}
	}
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
	for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
	{
		for(int y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for(int x = 0; x < CHUNK_BLOCKS; ++x)
			{
				int block;
				file >> block;
				blockData[BLOCK_INDEX(x, y, z)] = (BlockID)block;
			}
		}
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
	if(chunk->isModified()) {
		saveBlockData(util::getAbsoluteFilePath(World::getWorldPath() + "/Chunks/" + util::intToStr(itr->first) + ".obj"), chunk->m_blocks);
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
		m_chunkPool[i] = new Chunk();
	}

	// Set optimal chunk count
	m_optimalChunkCount = optimalChunkCount;
}

void ChunkLoader::loadActiveArea()
{
	Chunk *chunk;
	for(int y = m_activeArea.y0-1; y <= m_activeArea.y1+1; y++)
	{
		for(int x = m_activeArea.x0-1; x <= m_activeArea.x1+1; x++)
		{
			if(chunk = &getChunkAt(x, y))
			{
				chunk->generateVertexBuffer(this, TERRAIN_LAYER_BACK);
				chunk->generateVertexBuffer(this, TERRAIN_LAYER_MIDDLE);
				chunk->generateVertexBuffer(this, TERRAIN_LAYER_FRONT);
			}
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
	string chunkFilePath = World::getWorldPath() + "/Chunks/" + util::intToStr(key) + ".obj";

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

float round(float d)
{
	return floor(d + 0.5f);
}

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
	World::getDebug()->setVariable("Chunk load direction", "[" + util::floatToStr(dir.x) + ", " + util::floatToStr(dir.y) + "]");

	if(m_chunks.size() >= m_optimalChunkCount)
	{
		// Free inactive chunks
		freeInactiveChunk();
	}

	// Load offscren chunks
	Vector2i centerChunkPosition(floor(center.x/CHUNK_PXF), floor(center.y/CHUNK_PXF));
	Chunk &chunk = getChunkAt(centerChunkPosition.x + m_circleLoadPattern[m_circleLoadIndex].x, centerChunkPosition.y + m_circleLoadPattern[m_circleLoadIndex].y);
	if(chunk.isDirty(TERRAIN_LAYER_BACK)) chunk.generateVertexBuffer(this, TERRAIN_LAYER_BACK);
	if(chunk.isDirty(TERRAIN_LAYER_MIDDLE)) chunk.generateVertexBuffer(this, TERRAIN_LAYER_MIDDLE);
	if(chunk.isDirty(TERRAIN_LAYER_FRONT)) chunk.generateVertexBuffer(this, TERRAIN_LAYER_FRONT);
	m_circleLoadIndex = (m_circleLoadIndex + 1) % m_circleLoadPattern.size();
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
	int loadAreaWidth  = (int)(floor(width *0.5f/CHUNK_PXF) * 2 + 3) + m_loadAreaRadius * 2;
	int loadAreaHeight = (int)(floor(height*0.5f/CHUNK_PXF) * 2 + 3) + m_loadAreaRadius * 2;

	setOptimalChunkCount(loadAreaWidth * loadAreaHeight);
	
	priority_queue<Vector2i, vector<Vector2i>, VectorComparator> minHeap;
	for(int y = -floor(loadAreaHeight*0.5f) + 1; y < floor(loadAreaHeight*0.5f); ++y)
	{
		for(int x = -floor(loadAreaWidth*0.5f) + 1; x < floor(loadAreaWidth*0.5f); ++x)
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