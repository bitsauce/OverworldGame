#include "terrain.h"
#include "constants.h"
#include "game/debug.h"
#include "game/camera.h"
#include "game/world.h"
#include "Blocks/BlockData.h"
#include "BlockEntities/BlockEntityData.h"
#include "lighting/spotlight.h"

Terrain::Terrain() :
	GameObject(DRAW_ORDER_TERRAIN),
	m_chunkLoader(World::getCamera())
{
	xd::LOG("Initializing terrain");

	// Window
	xd::Window::addWindowListener(this);
	resizeEvent(xd::Window::getSize().x, xd::Window::getSize().y);

	Spotlight::s_vertices = new xd::Vertex[SPOTLIGHT_SEGMENTS+2];
	
	// Get terrain seed
	TerrainGen::s_seed = xd::Random().nextInt();
}

Terrain::~Terrain()
{
	xd::Window::removeWindowListener(this);
}
	
// Move?
void Terrain::saveChunks()
{
	xd::LOG("Saving chunks...");

	// Iterate loaded chunks
	/*for(unordered_map<uint, TerrainChunk*>::iterator itr = m_chunks.begin(); itr != m_chunks.end(); ++itr)
	{
		// Skip unmodified chunks
		if(!itr->second->m_modified) continue;

		// Save chunk
		string path = World::getWorldPath() + "/chunks/" + xd::util::intToStr(CHUNK_KEY(itr->second->getX(), itr->second->getY())) + ".obj";
		xd::FileWriter writer(path);
		if(!writer)
		{
			xd::LOG("Error opening chunk file: '%s'", path);
			continue;
		}
		itr->second->serialize(writer);
	}*/
}
	
void Terrain::load(const xd::IniFile &file)
{
	xd::LOG("Loading terrain...");
		
	TerrainGen::s_seed = 0;// parseInt(file.getValue("terrain", "seed"));
}
	
// BLOCKS
bool Terrain::setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return m_chunkLoader.getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF)).setBlockAt(xd::math::mod(x, CHUNK_BLOCKS), xd::math::mod(y, CHUNK_BLOCKS), block, layer);
}

BlockID Terrain::getBlockAt(const int x, const int y, const TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return m_chunkLoader.getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF)).getBlockAt(xd::math::mod(x, CHUNK_BLOCKS), xd::math::mod(y, CHUNK_BLOCKS), layer);
}
	
bool Terrain::isBlockAt(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return getBlockAt(x, y, layer) != BLOCK_EMPTY;
}

bool Terrain::removeBlockAt(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return m_chunkLoader.getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF)).setBlockAt(xd::math::mod(x, CHUNK_BLOCKS), xd::math::mod(y, CHUNK_BLOCKS), BLOCK_EMPTY, layer);
}

// BLOCK ENTITIES
bool Terrain::setBlockEntityAt(const int x, const int y, BlockEntityID blockEntity)
{
	return BlockEntityData::get(blockEntity).tryPlace(x, y);
}

// UPDATING
void Terrain::update()
{
}
	
// DRAWING
void Terrain::draw(xd::SpriteBatch *spriteBatch)
{
	spriteBatch->end();
	spriteBatch->begin(xd::SpriteBatch::State(xd::SpriteBatch::DEFERRED, xd::BlendState::PRESET_ALPHA_BLEND, World::getCamera()->getProjectionMatrix()));

	xd::GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();

	ChunkLoader::ChunkArea area = m_chunkLoader.getActiveArea();
	gfxContext.setTexture(BlockData::getBlockAtlas()->getTexture());
	gfxContext.setProjectionMatrix(World::getCamera()->getProjectionMatrix());
	for(int y = area.y0; y <= area.y1; ++y)
	{
		for(int x = area.x0; x <= area.x1; ++x)
		{
			TerrainChunk &chunk = m_chunkLoader.getChunkAt(x, y);

			// Should we generate new vertex buffers
			if(chunk.isDirty())
			{
				chunk.generateVertexBuffers(&m_chunkLoader);
			}

			Matrix4 mat;
			mat.scale(BLOCK_PXF, BLOCK_PXF, 1.0f);
			mat.translate(x * CHUNK_PXF, y * CHUNK_PXF, 0.0f);

			gfxContext.pushMatrix(mat);
			chunk.draw(gfxContext);
			gfxContext.popMatrix();
		}
	}
}

// WINDOW
void Terrain::resizeEvent(uint width, uint height)
{
}