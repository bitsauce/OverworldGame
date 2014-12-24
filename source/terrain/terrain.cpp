#include "terrain.h"
#include "constants.h"
#include "game/debug.h"
#include "game/camera.h"
#include "game/world.h"
#include "game/blockdata.h"
#include "lighting/spotlight.h"

#define CHUNK_KEY(X, Y) ((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000)

Terrain::Terrain() :
	GameObject(DRAW_ORDER_TERRAIN),
	m_shadowPass0(nullptr),
	m_shadowPass1(nullptr),
	m_shadowPass2(nullptr),
	m_shadowRenderTarget(nullptr)
{
	LOG("Initializing terrain");

	//setDrawPass(DRAW_ORDER_TERRAIN, true);
	//setDrawPass(DRAW_ORDER_TERRAIN_SHADOWS, true);
	
	// Resize textures
	XWindow::addWindowListener(this);
	resizeEvent(XWindow::getSize().x, XWindow::getSize().y);

	// Shadow radius
	m_shadowRadius = 6;
	
	// Load blur shaders
	m_blurHShader = xd::ResourceManager::get<xd::Shader>(":/shaders/blur_h");
	m_blurVShader = xd::ResourceManager::get<xd::Shader>(":/shaders/blur_v");

	// Setup vertex format
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

// VERTEX FORMAT
xd::VertexFormat Terrain::getVertexFormat() const
{
	return vertexFormat;
}
	
// Move?
void Terrain::saveChunks()
{
	LOG("Saving chunks...");

	// Iterate loaded chunks
	for(unordered_map<uint, TerrainChunk*>::iterator itr = chunks.begin(); itr != chunks.end(); ++itr)
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
BlockID Terrain::getBlockAt(const int x, const int y, const TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	return getChunk(floor(x / CHUNK_BLOCKSF), floor(y / CHUNK_BLOCKSF)).getBlockAt(XMath::mod(x, CHUNK_BLOCKS), XMath::mod(y, CHUNK_BLOCKS), layer);
}
	
bool Terrain::isBlockAt(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	return getBlockAt(x, y, layer) != BLOCK_EMPTY;
}

// BLOCK MODIFICATION
bool Terrain::setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	return getChunk(floor(x / CHUNK_BLOCKSF), floor(y / CHUNK_BLOCKSF)).setBlockAt(XMath::mod(x, CHUNK_BLOCKS), XMath::mod(y, CHUNK_BLOCKS), block, layer);
}
	
bool Terrain::removeBlockAt(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	return getChunk(floor(x / CHUNK_BLOCKSF), floor(y / CHUNK_BLOCKSF)).setBlockAt(XMath::mod(x, CHUNK_BLOCKS), XMath::mod(y, CHUNK_BLOCKS), BLOCK_EMPTY, layer);
}

// CHUNKS
TerrainChunk &Terrain::getChunk(const int chunkX, const int chunkY, const bool generate)
{
	uint key = CHUNK_KEY(chunkX, chunkY);
	if(chunks.find(key) == chunks.end())
	{
		if(generate)
		{
			LOG("Chunk [%i, %i] added to queue", chunkX, chunkY);
			
			// Create new chunk
			TerrainChunk *chunk = 0;
			string chunkFile = World::getWorldPath() + "/chunks/" + util::intToStr(key) + ".obj";
			if(util::fileExists(chunkFile))
			{
				//@chunk = cast<TerrainChunk>(@Scripts.deserialize(chunkFile));
			}
			else
			{
				chunk = new TerrainChunk(chunkX, chunkY);

				// Inform all neightbour chunks about the new guy
				if(isChunk(chunkX,   chunkY+1)) { chunks[CHUNK_KEY(chunkX,   chunkY+1)]->m_nextChunk[0] = chunk; chunk->m_nextChunk[4] = chunks[CHUNK_KEY(chunkX,   chunkY+1)]; }
				if(isChunk(chunkX-1, chunkY+1)) { chunks[CHUNK_KEY(chunkX-1, chunkY+1)]->m_nextChunk[1] = chunk; chunk->m_nextChunk[5] = chunks[CHUNK_KEY(chunkX-1, chunkY+1)]; }
				if(isChunk(chunkX-1, chunkY  )) { chunks[CHUNK_KEY(chunkX-1, chunkY  )]->m_nextChunk[2] = chunk; chunk->m_nextChunk[6] = chunks[CHUNK_KEY(chunkX-1, chunkY  )]; }
				if(isChunk(chunkX-1, chunkY-1)) { chunks[CHUNK_KEY(chunkX-1, chunkY-1)]->m_nextChunk[3] = chunk; chunk->m_nextChunk[7] = chunks[CHUNK_KEY(chunkX-1, chunkY-1)]; }
				if(isChunk(chunkX,   chunkY-1)) { chunks[CHUNK_KEY(chunkX,   chunkY-1)]->m_nextChunk[4] = chunk; chunk->m_nextChunk[0] = chunks[CHUNK_KEY(chunkX,   chunkY-1)]; }
				if(isChunk(chunkX+1, chunkY-1)) { chunks[CHUNK_KEY(chunkX+1, chunkY-1)]->m_nextChunk[5] = chunk; chunk->m_nextChunk[1] = chunks[CHUNK_KEY(chunkX+1, chunkY-1)]; }
				if(isChunk(chunkX+1, chunkY  )) { chunks[CHUNK_KEY(chunkX+1, chunkY  )]->m_nextChunk[6] = chunk; chunk->m_nextChunk[2] = chunks[CHUNK_KEY(chunkX+1, chunkY  )]; }
				if(isChunk(chunkX+1, chunkY+1)) { chunks[CHUNK_KEY(chunkX+1, chunkY+1)]->m_nextChunk[7] = chunk; chunk->m_nextChunk[3] = chunks[CHUNK_KEY(chunkX+1, chunkY+1)]; }
			}
				
			chunks[key] = chunk;
			chunk->generate();
			
			return *chunk;
		}
		return m_dummyChunk; // Create dummy
	}
	return *chunks[key];
}

bool Terrain::isChunk(const int chunkX, const int chunkY) const
{
	return chunks.find(CHUNK_KEY(chunkX, chunkY)) != chunks.end();
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
	chunkLoadQueue.clear();
}
	
// UPDATING
void Terrain::update()
{
	if(XInput::getKeyState(XD_LMB))
	{
		setBlockAt(floor((World::getCamera()->getPosition().x + XInput::getPosition().x)/BLOCK_PXF), floor((World::getCamera()->getPosition().y + XInput::getPosition().y)/BLOCK_PXF), BLOCK_LEAF);
	}
	else if(XInput::getKeyState(XD_RMB))
	{
		setBlockAt(floor((World::getCamera()->getPosition().x + XInput::getPosition().x)/BLOCK_PXF), floor((World::getCamera()->getPosition().y + XInput::getPosition().y)/BLOCK_PXF), BLOCK_EMPTY);
	}

	int cx = floor(World::getCamera()->getX()/CHUNK_PXF);
	int cy = floor(World::getCamera()->getY()/CHUNK_PXF);
	TerrainChunk *chunk = 0;
	if((chunk = &getChunk(cx, cy, true))->getState() != CHUNK_INITIALIZED)
	{
		LOG("Insta-generate chunk [%i, %i]", cx, cy);
		//chunkLoadQueue.remove(chunk);
		chunk->generate();
	}
		
	if(!chunkLoadQueue.empty())
	{
		// Load queued chunk
		chunkLoadQueue.back()->generate();
		chunkLoadQueue.pop_back();
	}

	Debug::setVariable("Chunks", util::intToStr(chunks.size()));
}
	
// DRAWING
void Terrain::draw(xd::SpriteBatch *spriteBatch)
{
	xd::GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();

	int x0 = floor(World::getCamera()->getX()/CHUNK_PXF);
	int y0 = floor(World::getCamera()->getY()/CHUNK_PXF);
	int x1 = floor((World::getCamera()->getX() + World::getCamera()->getWidth())/CHUNK_PXF);
	int y1 = floor((World::getCamera()->getY() + World::getCamera()->getHeight())/CHUNK_PXF);
	
	//if(m_prevX0 != x0 || m_prevY0 != y0)
	{
		// Disable alpha blend when drawing to render targets
		gfxContext.disable(xd::GraphicsContext::BLEND);

		// Render shadows to texture (pass 0)
		gfxContext.setRenderTarget(m_shadowPass0);
		gfxContext.clear(xd::GraphicsContext::COLOR_BUFFER);
		xd::Vertex vertices[4];
		for(int y = y0-2; y <= y1+2; ++y)
		{
			for(int x = x0-2; x <= x1+2; ++x)
			{
				gfxContext.setTexture(getChunk(x, y, true).m_shadowMap);
				gfxContext.drawRectangle((x - x0 + 1) * CHUNK_BLOCKS, (y - y0 + 1) * CHUNK_BLOCKS, CHUNK_BLOCKS, CHUNK_BLOCKS);
			}
		}

		// Blur horizontally (pass 1)
		gfxContext.setRenderTarget(m_shadowPass1);
		gfxContext.clear(xd::GraphicsContext::COLOR_BUFFER);
		m_blurHShader->setSampler2D("u_texture", m_shadowPass0->getTexture());
		m_blurHShader->setUniform1i("u_width", m_shadowPass0->getWidth());
		gfxContext.setShader(m_blurHShader);
		gfxContext.drawRectangle(0, 0, m_shadowPass1->getWidth(), m_shadowPass1->getHeight());

		// Blur vertically (pass 2)
		gfxContext.setRenderTarget(m_shadowPass2);
		gfxContext.clear(xd::GraphicsContext::COLOR_BUFFER);
		m_blurVShader->setSampler2D("u_texture", m_shadowPass1->getTexture());
		m_blurVShader->setUniform1i("u_height", m_shadowPass0->getHeight());
		m_blurVShader->setUniform1f("u_exponent", 2.0);
		gfxContext.setShader(m_blurVShader);
		gfxContext.drawRectangle(0, 0, m_shadowPass2->getWidth(), m_shadowPass2->getHeight());
		
		// Re-enable alpha blending
		gfxContext.setRenderTarget(nullptr);
		gfxContext.setShader(nullptr);
		gfxContext.enable(xd::GraphicsContext::BLEND);
	}
	
	gfxContext.setTexture(BlockData::getBlockAtlas()->getTexture());
	gfxContext.setProjectionMatrix(spriteBatch->getState().projectionMatix);
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

	//m_prevX0 = x0; m_prevY0 = y0;

	gfxContext.setRenderTarget(m_shadowRenderTarget);
	gfxContext.clear(xd::GraphicsContext::COLOR_BUFFER);
	gfxContext.disable(xd::GraphicsContext::BLEND);

	gfxContext.setTexture(m_shadowPass2->getTexture());
	gfxContext.drawRectangle((x0-1)*CHUNK_PXF, (y0-1)*CHUNK_PXF, m_shadowPass2->getWidth()*BLOCK_PXF, m_shadowPass2->getHeight()*BLOCK_PXF);
	
	gfxContext.enable(xd::GraphicsContext::BLEND);


	gfxContext.setTexture(nullptr);
	gfxContext.setBlendState(xd::BlendState::PRESET_ADDITIVE);

	Spotlight::drawAll(gfxContext);

	gfxContext.setRenderTarget(nullptr);
	
	gfxContext.setProjectionMatrix(Matrix4());
	gfxContext.setBlendState(xd::BlendState::PRESET_MULTIPLY);
	gfxContext.setTexture(m_shadowRenderTarget->getTexture());
	gfxContext.drawRectangle(0, 0, m_shadowRenderTarget->getWidth(), m_shadowRenderTarget->getHeight());
	gfxContext.setBlendState(xd::BlendState::PRESET_ALPHA_BLEND);
	
	if(XInput::getKeyState(XD_KEY_Z))
	{
		gfxContext.setTexture(nullptr);
		if(XInput::getKeyState(XD_KEY_B))
		{
			int x0 = floor(World::getCamera()->getX()/BLOCK_PXF);
			int y0 = floor(World::getCamera()->getY()/BLOCK_PXF);
			int x1 = floor((World::getCamera()->getX() + World::getCamera()->getWidth())/BLOCK_PXF);
			int y1 = floor((World::getCamera()->getY() + World::getCamera()->getHeight())/BLOCK_PXF);

			for(int y = y0; y <= y1; ++y)
			{
				gfxContext.drawRectangle(x0 * BLOCK_PXF, y * BLOCK_PXF, (x1 - x0 + 1) * BLOCK_PXF, 1.0f / World::getCamera()->getZoom(), xd::Color(127, 127, 127, 255));
			}

			for(int x = x0; x <= x1; ++x)
			{
				gfxContext.drawRectangle(x * BLOCK_PXF, y0 * BLOCK_PXF, 1.0f / World::getCamera()->getZoom(), (y1 - y0 + 1) * BLOCK_PXF, xd::Color(127, 127, 127, 255));
			} 
		}

		for(int y = y0; y <= y1; ++y)
		{
			gfxContext.drawRectangle(x0 * CHUNK_PX, y * CHUNK_PX, (x1 - x0 + 1) * CHUNK_PX, 1.0f / World::getCamera()->getZoom(), xd::Color(0, 0, 0, 255));
		}

		for(int x = x0; x <= x1; ++x)
		{
			gfxContext.drawRectangle(x * CHUNK_PX, y0 * CHUNK_PX, 1.0f / World::getCamera()->getZoom(), (y1 - y0 + 1) * CHUNK_PX, xd::Color(0, 0, 0, 255));
		}
	}
}


void Terrain::resizeEvent(uint width, uint height)
{
	// Clear old render targets
	delete m_shadowPass0;
	delete m_shadowPass1;
	delete m_shadowPass2;
	delete m_shadowRenderTarget;

	// Create shadow textures
	uint targetWidth = (floor(width/CHUNK_PXF) + 4) * CHUNK_BLOCKS;
	uint targetHeight = (floor(height/CHUNK_PXF) + 4) * CHUNK_BLOCKS;
	m_shadowPass0 = new xd::RenderTarget2D(targetWidth, targetHeight, 1);
	m_shadowPass1 = new xd::RenderTarget2D(targetWidth, targetHeight, 1);
	m_shadowPass2 = new xd::RenderTarget2D(targetWidth, targetHeight, 1);
	m_shadowPass2->getTexture()->setFiltering(xd::Texture2D::LINEAR);
	m_shadowRenderTarget = new xd::RenderTarget2D(width, height);

	// Make sure the shadows will update
	m_prevX0 = m_prevY0 = 0xFFFFFFFF;
}