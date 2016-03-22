#include "Constants.h"
#include "BlockEntityData.h"
#include "Torch.h"

vector<BlockEntityData*> BlockEntityData::s_data(BLOCK_ENTITY_COUNT);
TextureAtlas *BlockEntityData::s_atlas;
Resource<Texture2D> BlockEntityData::s_blockEntityDataTexture = nullptr;

struct BlockEntityDescriptor
{
	const BlockEntityID id;
	const string name;
	const string texturePath;
	const uint width;
	const uint height;
	const uint frameCount;
	const uint animationSpeed; // 0 = 0%, 255 = Every tick
	const function<BlockEntity*(World*, const int, const int, const BlockEntityData*)> factory;
};

BlockEntity* dummy(World*, const int, const int, const BlockEntityData*)
{
	return 0;
}

static BlockEntityDescriptor g_blockEntityData[] = {
	{ BLOCK_ENTITY_NULL, "NULL", "Sprites/Blocks/Empty.png", 1, 1, 1, 0, dummy },

	{ BLOCK_ENTITY_TORCH, "Torch", "Sprites/BlockEntities/LightSources/Torch_anim.png", 1, 1, 3, 255, Torch::Factory },
	{ BLOCK_ENTITY_TORCH_2, "Torch2", "Sprites/BlockEntities/LightSources/Torch_anim_2.png", 1, 1, 6, 0, Torch::Factory },

	{ BLOCK_ENTITY_COUNT, "", "", 0, 0, 0, 0, dummy }
};

void BlockEntityData::init()
{
	// Block entity data pixmap
	Pixmap blockDataPixmap(BLOCK_ENTITY_COUNT, 2);
	uchar pixelData[4];

	// Load block entity data
	BlockEntityDescriptor *data = &g_blockEntityData[0];
	vector<Pixmap> pixmaps(BLOCK_ENTITY_COUNT);
	while(data->id != BLOCK_ENTITY_COUNT)
	{
		// Create block entity data objects
		Pixmap pixmap(data->texturePath);
		s_data[data->id] = new BlockEntityData(data->id, data->name, pixmap, data->width, data->height, data->frameCount, data->factory);
		pixmaps[data->id] = pixmap;

		pixelData[0] = data->width * BLOCK_PXF;
		pixelData[1] = data->height * BLOCK_PXF;
		pixelData[2] = data->frameCount;
		pixelData[3] = data->animationSpeed;
		blockDataPixmap.setPixel(data->id, 1, pixelData);

		// Next block entity
		data++;
	}

	// Create block entity texture atlas
	s_atlas = new TextureAtlas(pixmaps);

	// Fill block entity UV data
	for(uint i = 0; i < BLOCK_ENTITY_COUNT; ++i)
	{
		Vector2I pos = s_atlas->get(i).uv0 * s_atlas->getTexture()->getSize();

		pixelData[0] = uchar(pos.x & 0xFF);
		pixelData[1] = uchar((pos.x >> 8) & 0xFF);

		pixelData[2] = uchar(pos.y & 0xFF);
		pixelData[3] = uchar((pos.y >> 8) & 0xFF);

		blockDataPixmap.setPixel(i, 0, pixelData);
	}
	s_blockEntityDataTexture = Resource<Texture2D>(new Texture2D(blockDataPixmap));
	s_blockEntityDataTexture->setFiltering(Texture2D::NEAREST);
}