#include "terrain.h"
#include "constants.h"
#include "game/debug.h"
#include "game/camera.h"
#include "game/world.h"
#include "game/blockdata.h"
#include "lighting/spotlight.h"

#define CHUNK_KEY(X, Y) ((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000)

Terrain::Terrain() :
	GameObject(DRAW_ORDER_TERRAIN)
{
	LOG("Initializing terrain");

	// Set max chunks to some value
	setMaxChunks(512);

	// Window
	XWindow::addWindowListener(this);
	resizeEvent(XWindow::getSize().x, XWindow::getSize().y);

	// Setup vertex format
	xd::VertexFormat vertexFormat;
	vertexFormat.set(xd::VERTEX_POSITION, 2);
	vertexFormat.set(xd::VERTEX_TEX_COORD, 2);
	TerrainChunk::s_vertices = vertexFormat.createVertices(4*12*16*16*3);
	Spotlight::s_vertices = new xd::Vertex[SPOTLIGHT_SEGMENTS+2];
	
	// Get terrain seed
	TerrainGen::s_seed = XRandom().nextInt();
}

Terrain::~Terrain()
{
	XWindow::removeWindowListener(this);
}
	
// Move?
void Terrain::saveChunks()
{
	LOG("Saving chunks...");

	// Iterate loaded chunks
	for(unordered_map<uint, TerrainChunk*>::iterator itr = m_chunks.begin(); itr != m_chunks.end(); ++itr)
	{
		// Skip unmodified chunks
		if(!itr->second->m_modified) continue;

		// Save chunk
		string path = World::getWorldPath() + "/chunks/" + util::intToStr(CHUNK_KEY(itr->second->getX(), itr->second->getY())) + ".obj";
		XFileWriter writer(path);
		if(!writer)
		{
			LOG("Error opening chunk file: '%s'", path);
			continue;
		}
		itr->second->serialize(writer);
	}
}
	
void Terrain::load(const XIniFile &file)
{
	LOG("Loading terrain...");
		
	TerrainGen::s_seed = 0;// parseInt(file.getValue("terrain", "seed"));
}
	
// BLOCK HELPERS
BlockID Terrain::getBlockAt(const int x, const int y, const TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return getChunk(floor(x / CHUNK_BLOCKSF), floor(y / CHUNK_BLOCKSF)).getBlockAt(XMath::mod(x, CHUNK_BLOCKS), XMath::mod(y, CHUNK_BLOCKS), layer);
}
	
bool Terrain::isBlockAt(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return getBlockAt(x, y, layer) != BLOCK_EMPTY;
}

// BLOCK MODIFICATION
bool Terrain::setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return getChunk(floor(x / CHUNK_BLOCKSF), floor(y / CHUNK_BLOCKSF)).setBlockAt(XMath::mod(x, CHUNK_BLOCKS), XMath::mod(y, CHUNK_BLOCKS), block, layer);
}
	
bool Terrain::removeBlockAt(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return getChunk(floor(x / CHUNK_BLOCKSF), floor(y / CHUNK_BLOCKSF)).setBlockAt(XMath::mod(x, CHUNK_BLOCKS), XMath::mod(y, CHUNK_BLOCKS), BLOCK_EMPTY, layer);
}

// CHUNKS
TerrainChunk &Terrain::getChunk(const int chunkX, const int chunkY, const bool generate)
{
	uint key = CHUNK_KEY(chunkX, chunkY);
	if(m_chunks.find(key) == m_chunks.end())
	{
		if(generate)
		{
			LOG("Chunk [%i, %i] added to queue", chunkX, chunkY);
			
			// Create new chunk
			TerrainChunk *chunk = nullptr;
			string chunkFile = World::getWorldPath() + "/chunks/" + util::intToStr(key) + ".obj";
			if(util::fileExists(chunkFile))
			{
				//@chunk = cast<TerrainChunk>(@Scripts.deserialize(chunkFile));
			}
			else
			{
				if(m_chunkPool.empty())
				{
					// Grab a chunk offscreen
					int x0 = floor(World::getCamera()->getX()/CHUNK_PXF);
					int y0 = floor(World::getCamera()->getY()/CHUNK_PXF);
					int x1 = floor((World::getCamera()->getX() + World::getCamera()->getWidth())/CHUNK_PXF);
					int y1 = floor((World::getCamera()->getY() + World::getCamera()->getHeight())/CHUNK_PXF);
					for(unordered_map<uint, TerrainChunk*>::iterator itr = m_chunks.begin(); itr != m_chunks.end(); ++itr)
					{
						int x = itr->second->getX(), y = itr->second->getY();
						if(x < x0 || x > x1 || y < y0 || y > y1)
						{
							chunk = itr->second;
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
				if(isChunk(chunkX,   chunkY+1)) { m_chunks[CHUNK_KEY(chunkX,   chunkY+1)]->m_nextChunk[0] = chunk; chunk->m_nextChunk[4] = m_chunks[CHUNK_KEY(chunkX,   chunkY+1)]; }
				if(isChunk(chunkX-1, chunkY+1)) { m_chunks[CHUNK_KEY(chunkX-1, chunkY+1)]->m_nextChunk[1] = chunk; chunk->m_nextChunk[5] = m_chunks[CHUNK_KEY(chunkX-1, chunkY+1)]; }
				if(isChunk(chunkX-1, chunkY  )) { m_chunks[CHUNK_KEY(chunkX-1, chunkY  )]->m_nextChunk[2] = chunk; chunk->m_nextChunk[6] = m_chunks[CHUNK_KEY(chunkX-1, chunkY  )]; }
				if(isChunk(chunkX-1, chunkY-1)) { m_chunks[CHUNK_KEY(chunkX-1, chunkY-1)]->m_nextChunk[3] = chunk; chunk->m_nextChunk[7] = m_chunks[CHUNK_KEY(chunkX-1, chunkY-1)]; }
				if(isChunk(chunkX,   chunkY-1)) { m_chunks[CHUNK_KEY(chunkX,   chunkY-1)]->m_nextChunk[4] = chunk; chunk->m_nextChunk[0] = m_chunks[CHUNK_KEY(chunkX,   chunkY-1)]; }
				if(isChunk(chunkX+1, chunkY-1)) { m_chunks[CHUNK_KEY(chunkX+1, chunkY-1)]->m_nextChunk[5] = chunk; chunk->m_nextChunk[1] = m_chunks[CHUNK_KEY(chunkX+1, chunkY-1)]; }
				if(isChunk(chunkX+1, chunkY  )) { m_chunks[CHUNK_KEY(chunkX+1, chunkY  )]->m_nextChunk[6] = chunk; chunk->m_nextChunk[2] = m_chunks[CHUNK_KEY(chunkX+1, chunkY  )]; }
				if(isChunk(chunkX+1, chunkY+1)) { m_chunks[CHUNK_KEY(chunkX+1, chunkY+1)]->m_nextChunk[7] = chunk; chunk->m_nextChunk[3] = m_chunks[CHUNK_KEY(chunkX+1, chunkY+1)]; }
			}
				
			m_chunks[key] = chunk;
			chunk->generate();
			
			return *chunk;
		}
		return m_dummyChunk; // Create dummy
	}
	return *m_chunks[key];
}

bool Terrain::isChunk(const int chunkX, const int chunkY) const
{
	return m_chunks.find(CHUNK_KEY(chunkX, chunkY)) != m_chunks.end();
}
	
void Terrain::loadVisibleChunks()
{
	int x0 = floor(World::getCamera()->getX()/CHUNK_PXF);
	int y0 = floor(World::getCamera()->getY()/CHUNK_PXF);
	int x1 = floor((World::getCamera()->getX() + World::getCamera()->getWidth())/CHUNK_PXF);
	int y1 = floor((World::getCamera()->getY() + World::getCamera()->getHeight())/CHUNK_PXF);
		
	TerrainChunk *chunk;
	for(int y = y0-1; y <= y1+1; y++)
	{
		for(int x = x0-1; x <= x1+1; x++)
		{
			if((chunk = &getChunk(x, y, true))->getState() != CHUNK_INITIALIZED)
			{
				chunk->generate();
			}
		}
	}
}

void Terrain::setMaxChunks(const uint maxChunkCount)
{
	m_chunkPool.resize(maxChunkCount);
	for(uint i = 0; i < maxChunkCount; ++i)
	{
		m_chunkPool[i] = new TerrainChunk();
	}
}

// UPDATING
void Terrain::update()
{
	int cx = floor(World::getCamera()->getX()/CHUNK_PXF);
	int cy = floor(World::getCamera()->getY()/CHUNK_PXF);
	TerrainChunk *chunk = 0;
	if((chunk = &getChunk(cx, cy, true))->getState() != CHUNK_INITIALIZED)
	{
		LOG("Insta-generate chunk [%i, %i]", cx, cy);
		chunk->generate();
	}

	World::getDebug()->setVariable("Chunks", util::intToStr(m_chunks.size()));
}
	
// DRAWING
void Terrain::draw(xd::SpriteBatch *spriteBatch)
{
	xd::GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();

	// Get chunk coordinates
	int x0 = floor(World::getCamera()->getX()/CHUNK_PXF);
	int y0 = floor(World::getCamera()->getY()/CHUNK_PXF);
	int x1 = floor((World::getCamera()->getX() + World::getCamera()->getWidth())/CHUNK_PXF);
	int y1 = floor((World::getCamera()->getY() + World::getCamera()->getHeight())/CHUNK_PXF);
	
	// Draw tiles
	//gfxContext.setRenderTarget(m_renderTarget);
	//gfxContext.clear(xd::GraphicsContext::COLOR_BUFFER);

	gfxContext.setTexture(BlockData::getBlockAtlas()->getTexture());
	gfxContext.setProjectionMatrix(World::getCamera()->getProjectionMatrix());
	for(int y = y0-1; y <= y1+1; ++y)
	{
		for(int x = x0-1; x <= x1+1; ++x)
		{
			Matrix4 mat;
			mat.scale(BLOCK_PXF, BLOCK_PXF, 1.0f);
			mat.translate(x * CHUNK_PXF, y * CHUNK_PXF, 0.0f);

			gfxContext.pushMatrix(mat);
			getChunk(x, y, true).draw(gfxContext);
			gfxContext.popMatrix();
		}
	}
	//gfxContext.setRenderTarget(nullptr);

	/*gfxContext.setProjectionMatrix(Matrix4());
	gfxContext.setTexture(m_renderTarget->getTexture());
	gfxContext.drawRectangle(0.0f, 0.0f, m_renderTarget->getWidth(), m_renderTarget->getHeight());
	gfxContext.setTexture(nullptr);
	gfxContext.setProjectionMatrix(World::getCamera()->getProjectionMatrix());

	gfxContext.setProjectionMatrix(Matrix4());
	gfxContext.setShader(m_directionalLightingShader);
	m_directionalLightingShader->setSampler2D("u_texture", m_renderTarget->getTexture());
	m_directionalLightingShader->setUniform1f("u_height", m_renderTarget->getHeight());
	gfxContext.drawRectangle(0.0f, 0.0f, m_renderTarget->getWidth(), m_renderTarget->getHeight());
	gfxContext.setShader(nullptr);
	gfxContext.setProjectionMatrix(World::getCamera()->getProjectionMatrix());*/
}

// WINDOW
void Terrain::resizeEvent(uint width, uint height)
{
	//delete m_renderTarget;
	//m_renderTarget = new xd::RenderTarget2D(width, height);
}