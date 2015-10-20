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

void Terrain::Drawer::draw(SpriteBatch *spriteBatch, const float alpha)
{
	// Flush to set the draw order straight
	spriteBatch->flush();

	// Setup graphics context
	GraphicsContext &context = spriteBatch->getGraphicsContext();
	context.setModelViewMatrix(m_camera->getModelViewMatrix(alpha));

	// Draw chunk area
	ChunkLoader::ChunkArea area = m_chunkLoader->getActiveArea();

	context.setBlendState(BlendState(BlendState::BLEND_ONE, BlendState::BLEND_ONE_MINUS_SRC_ALPHA));
	context.setShader(m_chunkLoader->m_tileMapShader);
	m_chunkLoader->m_tileMapShader->setSampler2D("u_SortedBlockTexture", m_chunkLoader->m_sortedBlocksRenderTarget[m_layer]->getTexture(0));
	m_chunkLoader->m_tileMapShader->setSampler2D("u_SortedQuadTexture", m_chunkLoader->m_sortedBlocksRenderTarget[m_layer]->getTexture(1));

	float u0 = m_chunkLoader->m_globalChunkPosition.x / (float) area.getWidth(),
		v0 = -m_chunkLoader->m_globalChunkPosition.y / (float) area.getHeight(),
		u1 = u0 + 1.0f,
		v1 = v0 + 1.0f;

	context.drawRectangle(area.x0 * CHUNK_PXF, area.y0 * CHUNK_PXF, (area.x1 - area.x0 + 1) * CHUNK_PXF, (area.y1 - area.y0 + 1) * CHUNK_PXF, Color(255), TextureRegion(u0, v0, u1, v1));
	context.setShader(0);
	context.setBlendState(BlendState(BlendState::PRESET_ALPHA_BLEND));
}