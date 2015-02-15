#include "terrain.h"
#include "constants.h"
#include "game/debug.h"
#include "game/camera.h"
#include "game/world.h"
#include "Blocks/BlockData.h"
#include "BlockEntities/BlockEntityData.h"
#include "lighting/spotlight.h"

Terrain::Terrain() :
	m_chunkLoader(World::getCamera()),
	m_background(this, DRAW_ORDER_TERRAIN_BACKGROUND, TERRAIN_LAYER_BACK),
	m_middleground(this, DRAW_ORDER_TERRAIN_MIDDLEGROUND, TERRAIN_LAYER_MIDDLE),
	m_foreground(this, DRAW_ORDER_TERRAIN_FOREGROUND, TERRAIN_LAYER_FRONT)
{
	LOG("Initializing terrain");

	// Window
	Window::addWindowListener(this);
	resizeEvent(Window::getSize().x, Window::getSize().y);

	Spotlight::s_vertices = new Vertex[SPOTLIGHT_SEGMENTS+2];
	
	// Get terrain seed
	TerrainGen::s_seed = Random().nextInt();
}

Terrain::~Terrain()
{
	Window::removeWindowListener(this);
}
	
// Move?
void Terrain::saveChunks()
{
	LOG("Saving chunks...");

	// Iterate loaded chunks
	/*for(unordered_map<uint, TerrainChunk*>::iterator itr = m_chunks.begin(); itr != m_chunks.end(); ++itr)
	{
		// Skip unmodified chunks
		if(!itr->second->m_modified) continue;

		// Save chunk
		string path = World::getWorldPath() + "/chunks/" + util::intToStr(CHUNK_KEY(itr->second->getX(), itr->second->getY())) + ".obj";
		FileWriter writer(path);
		if(!writer)
		{
			LOG("Error opening chunk file: '%s'", path);
			continue;
		}
		itr->second->serialize(writer);
	}*/
}
	
void Terrain::load(const IniFile &file)
{
	LOG("Loading terrain...");
		
	TerrainGen::s_seed = 0;// parseInt(file.getValue("terrain", "seed"));
}
	
// BLOCKS
bool Terrain::setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return m_chunkLoader.getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF)).setBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), block, layer);
}

BlockID Terrain::getBlockAt(const int x, const int y, const TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return m_chunkLoader.getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF)).getBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer);
}
	
bool Terrain::isBlockAt(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return getBlockAt(x, y, layer) != BLOCK_EMPTY;
}

bool Terrain::removeBlockAt(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	return m_chunkLoader.getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF)).setBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), BLOCK_EMPTY, layer);
}

// BLOCK ENTITIES
bool Terrain::setBlockEntityAt(const int x, const int y, BlockEntityID blockEntity)
{
	return BlockEntityData::get(blockEntity).tryPlace(x, y);
}

// DRAWING
Terrain::Drawer::Drawer(Terrain *terrain, const DrawOrder drawOrder, const TerrainLayer layer) :
	GameObject(drawOrder),
	m_chunkLoader(terrain->getChunkLoader()),
	m_layer(layer)
{
}

void Terrain::Drawer::draw(SpriteBatch *spriteBatch)
{
	// Flush to set the draw ordering straight
	spriteBatch->flush();

	// Setup graphics context
	GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();
	gfxContext.setTexture(BlockData::getBlockAtlas()->getTexture());
	gfxContext.setProjectionMatrix(World::getCamera()->getProjectionMatrix());

	// Draw chunk area
	ChunkLoader::ChunkArea area = m_chunkLoader->getActiveArea();
	for(int y = area.y0; y <= area.y1; ++y)
	{
		for(int x = area.x0; x <= area.x1; ++x)
		{
			TerrainChunk &chunk = m_chunkLoader->getChunkAt(x, y);

			// Should we generate new vertex buffers?
			if(chunk.isDirty(m_layer)) {
				chunk.generateVertexBuffer(m_chunkLoader, m_layer);
			}

			// Apply block-space matrix
			Matrix4 mat;
			mat.scale(BLOCK_PXF, BLOCK_PXF, 1.0f);
			mat.translate(x * CHUNK_PXF, y * CHUNK_PXF, 0.0f);

			// Draw chunk
			gfxContext.pushMatrix(mat);
			chunk.draw(gfxContext, m_layer);
			gfxContext.popMatrix();
		}
	}
}