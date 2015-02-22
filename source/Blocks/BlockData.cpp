#include "BlockData.h"
#include "Game/World.h"
#include "Terrain/Terrain.h"

vector<BlockData*> BlockData::s_blockData(BLOCK_COUNT);
TextureAtlas *BlockData::s_blockAtlas = nullptr;

void BlockData::init()
{
	Texture2DPtr temp = ResourceManager::get<Texture2D>(":/sprites/blocks/template.png");
	s_blockData[BLOCK_EMPTY] = new BlockData(BLOCK_EMPTY, ResourceManager::get<Texture2D>(":/sprites/blocks/empty.png"), ITEM_NONE, 0.0f);
	s_blockData[BLOCK_ENTITY] = new BlockData(BLOCK_ENTITY, ResourceManager::get<Texture2D>(":/sprites/blocks/empty.png"), ITEM_NONE, 0.0f);
	s_blockData[BLOCK_WOOD] = new BlockData(BLOCK_WOOD, ResourceManager::get<Texture2D>(":/sprites/blocks/oak_bark.png"), ITEM_OAK_BLOCK, 0.75f);
	s_blockData[BLOCK_LEAF] = new BlockData(BLOCK_LEAF, ResourceManager::get<Texture2D>(":/sprites/blocks/oak_leaf.png"), ITEM_LEAF_BLOCK, 0.0f);
	s_blockData[BLOCK_GRASS] = new BlockData(BLOCK_GRASS, ResourceManager::get<Texture2D>(":/sprites/blocks/grass.png"), ITEM_GRASS_BLOCK, 1.0f);
	s_blockData[BLOCK_DIRT_BACK] = new BlockData(BLOCK_DIRT_BACK, ResourceManager::get<Texture2D>(":/sprites/blocks/dirt_back.png"), ITEM_STONE_BLOCK, 0.95f);
	s_blockData[BLOCK_STONE] = new BlockData(BLOCK_STONE, ResourceManager::get<Texture2D>(":/sprites/blocks/stone.png"), ITEM_STONE_BLOCK, 1.0f);

	vector<Texture2DPtr> textures;
	for(uint i = 0; i < BLOCK_COUNT; ++i)
	{
		textures.push_back(s_blockData[i]->m_texture ? s_blockData[i]->m_texture : temp);
	}

	s_blockAtlas = new TextureAtlas(textures);
}

BlockData::BlockData(BlockID id, const Texture2DPtr &texture, const ItemID item, const float opacity) :
	m_id(id),
	m_texture(texture),
	m_item(item),
	m_opacity(opacity)
{
}

BlockData &BlockData::get(const BlockID block)
{
	return *s_blockData[block];
}