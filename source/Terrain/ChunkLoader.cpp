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
				chunk->generate();
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
			
			// Create new chunk
			TerrainChunk *chunk = nullptr;
			string chunkFile = World::getWorldPath() + "/chunks/" + xd::util::intToStr(key) + ".obj";
			if(xd::util::fileExists(chunkFile))
			{
				//@chunk = cast<TerrainChunk>(@Scripts.deserialize(chunkFile));
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

				chunk->load(chunkX, chunkY);

				// Inform all neightbour chunks about the new guy
				if(isChunkLoadedAt(chunkX,   chunkY+1)) { m_chunks[CHUNK_KEY(chunkX,   chunkY+1)]->m_nextChunk[0] = chunk; chunk->m_nextChunk[4] = m_chunks[CHUNK_KEY(chunkX,   chunkY+1)]; }
				if(isChunkLoadedAt(chunkX-1, chunkY+1)) { m_chunks[CHUNK_KEY(chunkX-1, chunkY+1)]->m_nextChunk[1] = chunk; chunk->m_nextChunk[5] = m_chunks[CHUNK_KEY(chunkX-1, chunkY+1)]; }
				if(isChunkLoadedAt(chunkX-1, chunkY  )) { m_chunks[CHUNK_KEY(chunkX-1, chunkY  )]->m_nextChunk[2] = chunk; chunk->m_nextChunk[6] = m_chunks[CHUNK_KEY(chunkX-1, chunkY  )]; }
				if(isChunkLoadedAt(chunkX-1, chunkY-1)) { m_chunks[CHUNK_KEY(chunkX-1, chunkY-1)]->m_nextChunk[3] = chunk; chunk->m_nextChunk[7] = m_chunks[CHUNK_KEY(chunkX-1, chunkY-1)]; }
				if(isChunkLoadedAt(chunkX,   chunkY-1)) { m_chunks[CHUNK_KEY(chunkX,   chunkY-1)]->m_nextChunk[4] = chunk; chunk->m_nextChunk[0] = m_chunks[CHUNK_KEY(chunkX,   chunkY-1)]; }
				if(isChunkLoadedAt(chunkX+1, chunkY-1)) { m_chunks[CHUNK_KEY(chunkX+1, chunkY-1)]->m_nextChunk[5] = chunk; chunk->m_nextChunk[1] = m_chunks[CHUNK_KEY(chunkX+1, chunkY-1)]; }
				if(isChunkLoadedAt(chunkX+1, chunkY  )) { m_chunks[CHUNK_KEY(chunkX+1, chunkY  )]->m_nextChunk[6] = chunk; chunk->m_nextChunk[2] = m_chunks[CHUNK_KEY(chunkX+1, chunkY  )]; }
				if(isChunkLoadedAt(chunkX+1, chunkY+1)) { m_chunks[CHUNK_KEY(chunkX+1, chunkY+1)]->m_nextChunk[7] = chunk; chunk->m_nextChunk[3] = m_chunks[CHUNK_KEY(chunkX+1, chunkY+1)]; }
			}
				
			m_chunks[key] = chunk;
			chunk->generate();
			
			return *chunk;
		}
		return m_dummyChunk; // Return dummy
	}
	return *m_chunks[key];
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
	// Update active chunk area
	Vector2 position = m_camera->getPosition();
	Vector2 size = m_applyZoom ? m_camera->getSizeZoomed() : m_camera->getSize();

	m_activeArea.x0 = (int)floor(position.x/CHUNK_PXF);
	m_activeArea.y0 = (int)floor(position.y/CHUNK_PXF);
	m_activeArea.x1 = (int)floor((position.x+size.x)/CHUNK_PXF);
	m_activeArea.y1 = (int)floor((position.x+size.y)/CHUNK_PXF);

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