#include "BitStream.h"
#include "Networking/Server.h"
#include "Networking/Client.h"

#include "Terrain.h"
#include "Constants.h"
#include "Blocks/BlockData.h"

#include "World/World.h"
#include "World/Lighting/Pointlight.h"
#include "World/Camera.h"

#include "Entities/Dynamic/ItemDrop.h"

Terrain::Terrain(World *world) :
	m_world(world),
	m_chunkLoader(world),
	m_background(this, world->getCamera(), PRIORITY_TERRAIN_BACKGROUND, TERRAIN_LAYER_BACK),
	m_middleground(this, world->getCamera(), PRIORITY_TERRAIN_MIDDLEGROUND, TERRAIN_LAYER_MIDDLE),
	m_foreground(this, world->getCamera(), PRIORITY_TERRAIN_FOREGROUND, TERRAIN_LAYER_FRONT)
{
	LOG("Initializing terrain");

	// Window
	resizeEvent(Window::getSize().x, Window::getSize().y);

	Pointlight::s_vertices = new Vertex[POINTLIGHT_SEGMENTS+2];
}

Terrain::~Terrain()
{
}
	
// BLOCKS
bool Terrain::setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer = TERRAIN_LAYER_MIDDLE)
{
	if(Connection::getInstance()->isServer())
	{
		RakNet::BitStream bitStream;
		bitStream.Write((RakNet::MessageID)ID_SET_BLOCK);
		bitStream.Write(x);
		bitStream.Write(y);
		bitStream.Write(block);
		bitStream.Write(layer);
		((Server*)Connection::getInstance())->sendPacket(&bitStream);
	}
	else if(Client::getInstance())
	{
		RakNet::BitStream bitStream;
		bitStream.Write((RakNet::MessageID)ID_SET_BLOCK);
		bitStream.Write(x);
		bitStream.Write(y);
		bitStream.Write(block);
		bitStream.Write(layer);
		((Client*)Connection::getInstance())->sendPacket(&bitStream);
	}

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
	BlockID blockID = getBlockAt(x, y, layer);
	if(setBlockAt(x, y, BLOCK_EMPTY, layer))
	{
		ItemDrop *itemDrop = new ItemDrop(m_world, BlockData::get(blockID).getItem());
		itemDrop->setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
		return true;
	}
	return false;
}

#include "Entities/EntityData.h"
#include "Entities/Static/StaticEntity.h"

void Terrain::placeStaticEntity(StaticEntity * entity)
{
	Vector2i pos = entity->getPosition();
	Vector2i size = ((StaticEntityData*)EntityData::Get(entity->getID()))->getSize();
	m_chunkLoader.getChunkAt((int) floor(pos.x / CHUNK_BLOCKSF), (int) floor(pos.y / CHUNK_BLOCKSF)).addStaticEntity(entity);
	for(int y = pos.y; y < pos.y + size.y; ++y)
	{
		for(int x = pos.x; x < pos.x + size.x; ++x)
		{
			m_chunkLoader.getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF)).setBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), BLOCK_ENTITY, TERRAIN_LAYER_MIDDLE);
		}
	}
}

// DRAWING
Terrain::Drawer::Drawer(Terrain *terrain, Camera *camera, const Priority drawOrder, const TerrainLayer layer) :
	m_chunkLoader(terrain->getChunkLoader()),
	m_camera(camera),
	m_layer(layer)
{
}

void Terrain::Drawer::draw(SpriteBatch *spriteBatch)
{
	// Flush to set the draw order straight
	spriteBatch->flush();

	// Setup graphics context
	GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();
	gfxContext.setModelViewMatrix(m_camera->getProjectionMatrix());

	// Draw chunk area
	ChunkLoader::ChunkArea area = m_chunkLoader->getActiveArea();
	for(int y = area.y0; y <= area.y1; ++y)
	{
		for(int x = area.x0; x <= area.x1; ++x)
		{
			Chunk &chunk = m_chunkLoader->getChunkAt(x, y);

			// Should we generate a new tile map?
			if(chunk.isDirty(m_layer))
			{
				chunk.updateTileMap(m_chunkLoader, m_layer);
			}

			// Draw chunk
			chunk.draw(gfxContext, m_layer);
		}
	}
}