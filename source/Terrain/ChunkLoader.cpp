#include "ChunkLoader.h"
#include "Game.h"

#define CHUNK_KEY(X, Y) ((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000)

ChunkLoader::ChunkLoader(Camera *camera) :
	GameObject(DRAW_ORDER_CHUNK_LOADER),
	m_applyZoom(true),
	m_camera(camera),
	m_chunkPositionIndex(0),
	m_loadAreaRadius(5)
{
	// Setup vertex format
	xd::VertexFormat vertexFormat;
	vertexFormat.set(xd::VERTEX_POSITION, 2);
	vertexFormat.set(xd::VERTEX_TEX_COORD, 2);
	TerrainChunk::s_vertices = vertexFormat.createVertices(4*12*16*16*3);

	// Set max chunks to some value
	setOptimalChunkCount(512);

	// Add window listener
	xd::Window::addWindowListener(this);
}

void ChunkLoader::setOptimalChunkCount(const uint optimalChunkCount)
{
	// Clear old chunks
	for(uint i = 0; i < m_chunkPool.size(); ++i)
	{
		delete m_chunkPool[i];
	}

	// Create new chunks
	m_chunkPool.resize(optimalChunkCount * 1.1f);
	for(uint i = 0; i < m_chunkPool.size(); ++i)
	{
		m_chunkPool[i] = new TerrainChunk();
	}

	// Set optimal chunk count
	m_optimalChunkCount = optimalChunkCount;
}

void ChunkLoader::loadActiveArea()
{
	TerrainChunk *chunk;
	for(int y = m_activeArea.y0-1; y <= m_activeArea.y1+1; y++)
	{
		for(int x = m_activeArea.x0-1; x <= m_activeArea.x1+1; x++)
		{
			if(chunk = &getChunkAt(x, y))
			{
				chunk->generateVertexBuffers(this);
			}
		}
	}
}

TerrainChunk &ChunkLoader::getChunkAt(const int chunkX, const int chunkY)
{
	uint key = CHUNK_KEY(chunkX, chunkY);
	if(m_chunks.find(key) == m_chunks.end())
	{
		// Load block data for this chunk
		return *loadChunkAt(chunkX, chunkY);
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
		if(m_chunkPool.empty())
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

	assert(chunk); // If we got here we probably ran out of chunks

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

	// Active area should have the same center as the view
	m_activeArea.x0 = (int)floor(center.x/CHUNK_PXF) - floor(size.x*0.5f/CHUNK_PXF) - 1;
	m_activeArea.y0 = (int)floor(center.y/CHUNK_PXF) - floor(size.y*0.5f/CHUNK_PXF) - 1;
	m_activeArea.x1 = (int)floor(center.x/CHUNK_PXF) + floor(size.x*0.5f/CHUNK_PXF) + 1;
	m_activeArea.y1 = (int)floor(center.y/CHUNK_PXF) + floor(size.y*0.5f/CHUNK_PXF) + 1;

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
		//m_circleIterator = m_circleLoadPattern.begin(); // Reset iterator
	}

	Vector2 dir = Vector2(chunkPosition) - m_averagePosition;
	World::getDebug()->setVariable("Chunk load direction", "[" + xd::util::floatToStr(dir.x) + ", " + xd::util::floatToStr(dir.y) + "]");

	// Find a chunk offscreen and add it to the pool
	for(unordered_map<uint, TerrainChunk*>::iterator itr = m_chunks.begin(); itr != m_chunks.end(); ++itr)
	{
		int x = itr->second->getX(), y = itr->second->getY();
		if(x < m_loadArea.x0-1 || x > m_loadArea.x1+1 || y < m_loadArea.y0-1 || y > m_loadArea.y1+1)
		{
			m_chunkPool.push_back(itr->second);
			m_chunks.erase(itr->first);
			break;
		}
	}
	
	if(m_chunks.size() < m_optimalChunkCount)
	{
		// Load offscren chunks
		/*Vector2i centerChunkPosition = center/CHUNK_PXF;
		TerrainChunk &chunk = getChunkAt(centerChunkPosition.x + (*m_circleIterator).x, centerChunkPosition.y + (*m_circleIterator).y);
		if(chunk.isDirty()) chunk.generateVertexBuffers(this);
		m_circleIterator++;*/

		bool generated = false;
		for(int y = m_loadArea.y0; y <= m_loadArea.y1 && !generated; ++y)
		{
			for(int x = m_loadArea.x0; x <= m_loadArea.x1 && !generated; ++x)
			{
				TerrainChunk &chunk = getChunkAt(x, y);
				if(chunk.isDirty())
				{
					chunk.generateVertexBuffers(this);
					generated = true;
				}
			}
		}
	}
}

void ChunkLoader::resizeEvent(uint width, uint height)
{
	int loadAreaWidth  = (int)(floor(width *0.5f/CHUNK_PXF) * 2 + 3) + m_loadAreaRadius * 2 + 2;
	int loadAreaHeight = (int)(floor(height*0.5f/CHUNK_PXF) * 2 + 3) + m_loadAreaRadius * 2 + 2;

	setOptimalChunkCount(loadAreaWidth * loadAreaHeight);
	
	/*int diagonalHalfLength = sqrt(loadAreaWidth*loadAreaWidth + loadAreaHeight*loadAreaHeight) * 0.5f;
	for(int r = 1; r < diagonalHalfLength; ++r)
	{
		for(int y = -r; y <= r; ++y)
		{
			for(int x = -r; x <= r; ++x)
			{
				if(abs(x) > loadAreaWidth*0.5f || abs(y) > loadAreaHeight*0.5f) continue;
				if((int)sqrt(x*x+y*y) == r)
				{
					m_circleLoadPattern.insert(Vector2i(x, y));
				}
			}
		}
	}
	m_circleIterator = m_circleLoadPattern.begin();*/
}