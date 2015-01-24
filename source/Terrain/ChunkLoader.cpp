#include "ChunkLoader.h"
#include "Game.h"

#define CHUNK_KEY(X, Y) ((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000)

ChunkLoader::ChunkLoader(Camera *camera) :
	GameObject(DRAW_ORDER_CHUNK_LOADER),
	m_applyZoom(true),
	m_camera(camera)
{
	// Setup vertex format
	xd::VertexFormat vertexFormat;
	vertexFormat.set(xd::VERTEX_POSITION, 2);
	vertexFormat.set(xd::VERTEX_TEX_COORD, 2);
	TerrainChunk::s_vertices = vertexFormat.createVertices(4*12*16*16*3);

	// Set max chunks to some value
	setMaxChunkCount(512);

	update();
}

void ChunkLoader::setMaxChunkCount(const uint maxChunkCount)
{
	m_chunkPool.resize(maxChunkCount);
	for(uint i = 0; i < maxChunkCount; ++i)
	{
		m_chunkPool[i] = new TerrainChunk();
	}
	m_maxChunkCount = maxChunkCount;
}

void ChunkLoader::loadActiveArea()
{
	TerrainChunk *chunk;
	for(int y = m_activeArea.y0-1; y <= m_activeArea.y1+1; y++)
	{
		for(int x = m_activeArea.x0-1; x <= m_activeArea.x1+1; x++)
		{
			if((chunk = &getChunkAt(x, y, true))->getState() != CHUNK_INITIALIZED)
			{
				chunk->generateVBO();
			}
		}
	}
}

TerrainChunk &ChunkLoader::getChunkAt(const int chunkX, const int chunkY, const bool generate)
{
	uint key = CHUNK_KEY(chunkX, chunkY);
	if(m_chunks.find(key) == m_chunks.end())
	{
		if(generate)
		{
			xd::LOG("Chunk [%i, %i] added to queue", chunkX, chunkY);
			
			// Load this chunk
			TerrainChunk *chunk = loadChunkAt(chunkX, chunkY);

			// Load neighbour chunks
			if(!isChunkLoadedAt(chunkX,   chunkY+1)) { TerrainChunk *nextChunk = loadChunkAt(chunkX,   chunkY+1); nextChunk->m_nextChunk[0] = chunk; chunk->m_nextChunk[4] = nextChunk; }
			if(!isChunkLoadedAt(chunkX-1, chunkY+1)) { TerrainChunk *nextChunk = loadChunkAt(chunkX-1, chunkY+1); nextChunk->m_nextChunk[1] = chunk; chunk->m_nextChunk[5] = nextChunk; }
			if(!isChunkLoadedAt(chunkX-1, chunkY  )) { TerrainChunk *nextChunk = loadChunkAt(chunkX-1, chunkY  ); nextChunk->m_nextChunk[2] = chunk; chunk->m_nextChunk[6] = nextChunk; }
			if(!isChunkLoadedAt(chunkX-1, chunkY-1)) { TerrainChunk *nextChunk = loadChunkAt(chunkX-1, chunkY-1); nextChunk->m_nextChunk[3] = chunk; chunk->m_nextChunk[7] = nextChunk; }
			if(!isChunkLoadedAt(chunkX,   chunkY-1)) { TerrainChunk *nextChunk = loadChunkAt(chunkX,   chunkY-1); nextChunk->m_nextChunk[4] = chunk; chunk->m_nextChunk[0] = nextChunk; }
			if(!isChunkLoadedAt(chunkX+1, chunkY-1)) { TerrainChunk *nextChunk = loadChunkAt(chunkX+1, chunkY-1); nextChunk->m_nextChunk[5] = chunk; chunk->m_nextChunk[1] = nextChunk; }
			if(!isChunkLoadedAt(chunkX+1, chunkY  )) { TerrainChunk *nextChunk = loadChunkAt(chunkX+1, chunkY  ); nextChunk->m_nextChunk[6] = chunk; chunk->m_nextChunk[2] = nextChunk; }
			if(!isChunkLoadedAt(chunkX+1, chunkY+1)) { TerrainChunk *nextChunk = loadChunkAt(chunkX+1, chunkY+1); nextChunk->m_nextChunk[7] = chunk; chunk->m_nextChunk[3] = nextChunk; }

			return *chunk;
		}
		return m_dummyChunk; // Return dummy
	}
	return *m_chunks[key];
}

TerrainChunk *ChunkLoader::loadChunkAt(const int chunkX, const int chunkY)
{
	uint key = CHUNK_KEY(chunkX, chunkY);
	string chunkFile = World::getWorldPath() + "/chunks/" + xd::util::intToStr(key) + ".obj";
	TerrainChunk *chunk = nullptr;

	if(xd::util::fileExists(chunkFile))
	{
		// TODO: Load chunk from file
	}
	else
	{
		if(m_chunks.size() >= m_maxChunkCount)
		{
			// Grab a chunk offscreen
			for(unordered_map<uint, TerrainChunk*>::iterator itr = m_chunks.begin(); itr != m_chunks.end(); ++itr)
			{
				int x = itr->second->getX(), y = itr->second->getY();
				if(x < m_activeArea.x0 || x > m_activeArea.x1 || y < m_activeArea.y0 || y > m_activeArea.y1)
				{
					chunk = itr->second;
					m_chunks.erase(itr->first);
					break;
				}
			}
		}
		else
		{
			// Grab a chunk from the pool
			chunk = m_chunkPool.back();
			m_chunkPool.pop_back();
		}
	}

	assert(chunk);
	(m_chunks[key] = chunk)->load(chunkX, chunkY);
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
	return floor(d + 0.5);
}

void ChunkLoader::update()
{
	// Update active chunk area
	Vector2 center = m_camera->getCenter();
	Vector2 size = m_applyZoom ? m_camera->getSize() : xd::Window::getSize();

	m_activeAreaSize = size/CHUNK_PXF;

	// Active area should have the same center as the view
	m_activeArea.x0 = (int)floor(center.x/CHUNK_PXF) - floor(size.x*0.5f/CHUNK_PXF) - 1;
	m_activeArea.y0 = (int)floor(center.y/CHUNK_PXF) - floor(size.y*0.5f/CHUNK_PXF) - 1;
	m_activeArea.x1 = (int)floor(center.x/CHUNK_PXF) + floor(size.x*0.5f/CHUNK_PXF) + 1;
	m_activeArea.y1 = (int)floor(center.y/CHUNK_PXF) + floor(size.y*0.5f/CHUNK_PXF) + 1;

	// Update load area
	m_loadArea.x0 = m_activeArea.x0 - 5;
	m_loadArea.y0 = m_activeArea.y0 - 5;
	m_loadArea.x1 = m_activeArea.x1 + 5;
	m_loadArea.y1 = m_activeArea.y1 + 5;

	if(m_chunks.size() >= m_optimalChunkCount)
	{
		// Find a chunk offscreen and add it to the pool
		for(unordered_map<uint, TerrainChunk*>::iterator itr = m_chunks.begin(); itr != m_chunks.end(); ++itr)
		{
			int x = itr->second->getX(), y = itr->second->getY();
			if(x < m_activeArea.x0 || x > m_activeArea.x1 || y < m_activeArea.y0 || y > m_activeArea.y1)
			{
				m_chunkPool.push_back(itr->second);
				m_chunks.erase(itr->first);
				break;
			}
		}
	}
}