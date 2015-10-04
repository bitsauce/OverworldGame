#include "BlockData.h"
#include "World/World.h"
#include "World/Terrain/Terrain.h"

vector<BlockData*> BlockData::s_blockData(BLOCK_COUNT);
TextureAtlas *BlockData::s_blockAtlas = nullptr;

struct BlockDescriptor
{
	const BlockID id;
	const string texturePath;
	const ItemID itemID;
	const float opacity;
};

static BlockDescriptor g_blockData[] = {
	{ BLOCK_EMPTY, ":/Sprites/Blocks/Empty.png", ITEM_NONE, 0.0f },
	{ BLOCK_ENTITY, ":/Sprites/Blocks/Empty.png", ITEM_NONE, 0.0f },

	{ BLOCK_GRASS, ":/Sprites/Blocks/Grass.png", ITEM_BLOCK_DIRT, 1.0f },
	{ BLOCK_DIRT, ":/Sprites/Blocks/Dirt.png", ITEM_BLOCK_DIRT, 1.0f },
	{ BLOCK_DIRT_BACK, ":/Sprites/Blocks/DirtBack.png", ITEM_BLOCK_DIRT_BACK, 0.95f },

	{ BLOCK_OAK_WOOD, ":/Sprites/Blocks/OakWood.png", ITEM_BLOCK_OAK_WOOD, 0.75f },
	{ BLOCK_OAK_LEAVES, ":/Sprites/Blocks/OakLeaves.png", ITEM_BLOCK_OAK_LEAVES, 0.0f },

	{ BLOCK_STONE, ":/Sprites/Blocks/Stone.png", ITEM_BLOCK_STONE, 1.0f },

	{ BLOCK_COUNT, "", ITEM_NONE, 0.0f }
};

void BlockData::init()
{
	// Load block data
	BlockDescriptor *blockData = &g_blockData[0];
	while(blockData->id != BLOCK_COUNT)
	{
		s_blockData[blockData->id] = new BlockData(blockData->id, ResourceManager::get<Texture2D>(blockData->texturePath), blockData->itemID, blockData->opacity);
		blockData++;
	}

	// Create block texture atlas
	vector<Texture2DPtr> textures;
	for(uint i = 0; i < BLOCK_COUNT; ++i)
	{
		textures.push_back(s_blockData[i]->m_texture);
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