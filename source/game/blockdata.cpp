#include "blockdata.h"
#include "game/world.h"
#include "terrain/terrain.h"

vector<BlockData> BlockData::s_blockData(BLOCK_COUNT);
XTextureAtlas *BlockData::s_blockAtlas = nullptr;

void BlockData::init()
{
	shared_ptr<XTexture> temp = xd::ResourceManager::get<XTexture>(":/sprites/blocks/template.png");
	s_blockData[BLOCK_EMPTY] = BlockData(BLOCK_EMPTY, xd::ResourceManager::get<XTexture>(":/sprites/blocks/empty.png"), ITEM_NONE, 0.0f);
	s_blockData[BLOCK_OCCUPIED] = BlockData(BLOCK_OCCUPIED, xd::ResourceManager::get<XTexture>(":/sprites/blocks/empty.png"), ITEM_NONE, 0.0f);
	s_blockData[BLOCK_WOOD] = BlockData(BLOCK_WOOD, xd::ResourceManager::get<XTexture>(":/sprites/blocks/wood.png"), ITEM_WOOD_BLOCK, 0.75f);
	s_blockData[BLOCK_LEAF] = BlockData(BLOCK_LEAF, xd::ResourceManager::get<XTexture>(":/sprites/blocks/leaf.png"), ITEM_LEAF_BLOCK, 0.0f);
	s_blockData[BLOCK_GRASS] = BlockData(BLOCK_GRASS, xd::ResourceManager::get<XTexture>(":/sprites/blocks/grass.png"), ITEM_GRASS_BLOCK, 1.0f);
	s_blockData[BLOCK_STONE] = BlockData(BLOCK_STONE, xd::ResourceManager::get<XTexture>(":/sprites/blocks/stone.png"), ITEM_STONE_BLOCK, 1.0f);

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
	m_item(ITEM_NONE),
	m_opacity(0.0f)
{
}

BlockData::BlockData(BlockID id, const shared_ptr<XTexture> &texture, const ItemID item, const float opacity) :
	m_id(id),
	m_texture(texture),
	m_item(item),
	m_opacity(opacity)
{
}

BlockData &BlockData::get(const BlockID block)
{
	return s_blockData[block];
}