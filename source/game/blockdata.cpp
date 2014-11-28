#include "blockdata.h"
#include "game/world.h"
#include "terrain/terrain.h"

vector<BlockData> BlockData::s_blockData(BLOCK_COUNT);
XTextureAtlas *BlockData::s_blockAtlas = nullptr;

void BlockData::init()
{
	shared_ptr<XTexture> temp = xd::ResourceManager::get<XTexture>(":/sprites/tiles/tile_template.png");
	s_blockData[BLOCK_EMPTY] = BlockData(BLOCK_EMPTY, xd::ResourceManager::get<XTexture>(":/sprites/tiles/empty_tile.png"), 0/*ITEM_GRASS_BLOCK*/, 1.0f);
	s_blockData[BLOCK_SCENE_GRASS] = BlockData(BLOCK_SCENE_GRASS, xd::ResourceManager::get<XTexture>(":/sprites/tiles/grass_tile.png"), 0/*ITEM_GRASS_BLOCK*/, 1.0f);
	s_blockData[BLOCK_SCENE_STONE] = BlockData(BLOCK_SCENE_STONE, xd::ResourceManager::get<XTexture>(":/sprites/tiles/tile_template.png"), 0/*ITEM_GRASS_BLOCK*/, 1.0f);

	vector<shared_ptr<XTexture>> textures;
	for(uint i = 0; i < BLOCK_COUNT; ++i)
	{
		textures.push_back(s_blockData[i].m_texture ? s_blockData[i].m_texture : temp);
	}

	s_blockAtlas = new XTextureAtlas(textures);
}

BlockData::BlockData() :
	m_id(BLOCK_EMPTY),
	m_texture(0),
	m_itemID(0),
	m_opacity(0.0f)
{
}

BlockData::BlockData(BlockID id, const shared_ptr<XTexture> &texture, const /*ItemID*/ uint i, const float opacity) :
	m_id(id),
	m_texture(texture),
	m_itemID(i),
	m_opacity(opacity)
{
}

BlockData &BlockData::get(const BlockID block)
{
	return s_blockData[block];
}