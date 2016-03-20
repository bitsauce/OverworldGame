#include "BlockData.h"
#include "World/World.h"

vector<BlockData*> BlockData::s_blockData(BLOCK_COUNT);
TextureAtlas *BlockData::s_blockAtlas = nullptr;
Resource<Texture2D> BlockData::s_blockDataTexture = nullptr;

struct BlockDescriptor
{
	const BlockID id;
	const string imagePath;
	const ItemID itemID;
	const float opacity;
	const uint frameCount;
};

static BlockDescriptor g_blockData[] = {
	{ BLOCK_EMPTY, "Sprites/Blocks/Empty.png", ITEM_NONE, 0.0f, 1 },
	{ BLOCK_ENTITY, "Sprites/Blocks/Empty.png", ITEM_NONE, 0.0f, 1 },

	{ BLOCK_GRASS, "Sprites/Blocks/Grass.png", ITEM_BLOCK_DIRT, 1.0f, 1 },
	{ BLOCK_DIRT, "Sprites/Blocks/Dirt.png", ITEM_BLOCK_DIRT, 1.0f, 1 },
	{ BLOCK_DIRT_BACK, "Sprites/Blocks/DirtBack.png", ITEM_BLOCK_DIRT_BACK, 0.95f, 1 },

	{ BLOCK_OAK_WOOD, "Sprites/Blocks/OakWood.png", ITEM_BLOCK_OAK_WOOD, 0.75f, 1 },
	{ BLOCK_OAK_LEAVES, "Sprites/Blocks/OakLeaves.png", ITEM_BLOCK_OAK_LEAVES, 0.0f, 1 },

	{ BLOCK_STONE, "Sprites/Blocks/Stone.png", ITEM_BLOCK_STONE, 1.0f, 1 },
	{ BLOCK_ANIM_TEST, "Sprites/Blocks/AnimTest.png", ITEM_NONE, 1.0f, 2 },
	{ BLOCK_TORCH_TEST, "Sprites/Blocks/TorchTest.png", ITEM_NONE, 1.0f, 3 },

	{ BLOCK_COUNT, "", ITEM_NONE, 0.0f, 0 }
};

void BlockData::init()
{
	// Block data pixmap
	Pixmap blockDataPixmap(BLOCK_COUNT, 2);
	uchar pixelData[4];

	// Load block data
	BlockDescriptor *blockData = &g_blockData[0];
	vector<Pixmap> pixmaps(BLOCK_COUNT);
	while(blockData->id != BLOCK_COUNT)
	{
		// Create block data object
		Pixmap pixmap(blockData->imagePath, true);
		s_blockData[blockData->id] = new BlockData(blockData->id, pixmap, blockData->itemID, blockData->opacity);
		pixmaps[blockData->id] = pixmap;

		// Store meta data
		pixelData[0] = blockData->frameCount;
		pixelData[1] = 0;
		pixelData[2] = 0;
		pixelData[3] = 0;
		blockDataPixmap.setPixel(blockData->id, 1, pixelData);

		// Next block desc
		blockData++;
	}

	// Create block atlas
	s_blockAtlas = new TextureAtlas(pixmaps, 0);

	// Fill block UV data
	for(int id = 0; id < BLOCK_COUNT; ++id)
	{
		Vector2I pos = s_blockAtlas->get(id).uv0 * s_blockAtlas->getTexture()->getSize();

		pixelData[0] = uchar(pos.x & 0xFF);
		pixelData[1] = uchar((pos.x >> 8) & 0xFF);

		pixelData[2] = uchar(pos.y & 0xFF);
		pixelData[3] = uchar((pos.y >> 8) & 0xFF);

		blockDataPixmap.setPixel(id, 0, pixelData);
	}
	s_blockDataTexture = Resource<Texture2D>(new Texture2D(blockDataPixmap));
	s_blockDataTexture->setFiltering(Texture2D::NEAREST);
}

BlockData::BlockData(BlockID id, const Pixmap &pixmap, const ItemID item, const float opacity) :
	m_id(id),
	m_pixmap(pixmap),
	m_item(item),
	m_opacity(opacity)
{
}

BlockData *BlockData::get(const BlockID block)
{
	return s_blockData[block];
}