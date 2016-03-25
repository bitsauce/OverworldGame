#include "Constants.h"
#include "BlockEntityData.h"
#include "Torch.h"
#include "Bush.h"

vector<BlockEntityData*> BlockEntityData::s_data(BLOCK_ENTITY_COUNT);
TextureAtlas *BlockEntityData::s_textureAtlas;
Resource<Texture2D> BlockEntityData::s_dataTexture = nullptr;

struct BlockEntityDescriptor
{
	const BlockEntityID id;
	const string name;
	const string texturePath;
	const int width;
	const int height;
	const uint frameCount;
	const uint animationSpeed; // 0 = 0%, 255 = Every tick
	const WorldLayer layer;
	const uint placement; // NEED_WALL, NEED_ROOF, NEED_FLOOR, NEED_BACKGROUND, CAN_OVERLAP
	const function<BlockEntity*(World*, const int, const int, const BlockEntityData*)> factory;
};

BlockEntity* dummy(World*, const int, const int, const BlockEntityData*)
{
	return 0;
}

static BlockEntityDescriptor g_blockEntityData[] = {
	{ BLOCK_ENTITY_NULL, "NULL", "Sprites/Blocks/Empty.png", 1, 1, 1, 0, WORLD_LAYER_MIDDLE, true, dummy },

	{ BLOCK_ENTITY_TORCH, "Torch", "Sprites/BlockEntities/LightSources/Torch_anim_2.png", 1, 1, 3, 0, WORLD_LAYER_MIDDLE, false, Torch::Factory },
	{ BLOCK_ENTITY_BUSH, "Bush", "Sprites/BlockEntities/Vegetation/RedCurrantBush.png", 4, 2, 2, 0, WORLD_LAYER_MIDDLE, false, Bush::Factory },

	{ BLOCK_ENTITY_COUNT, "", "", 0, 0, 0, 0, (WorldLayer)0, false, dummy }
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
		s_data[data->id] = new BlockEntityData(data->id, data->name, pixmap, data->width, data->height, data->frameCount, data->layer, data->placement, data->factory);
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
	s_textureAtlas = new TextureAtlas(pixmaps);

	// Fill block entity UV data
	for(uint i = 0; i < BLOCK_ENTITY_COUNT; ++i)
	{
		Vector2I pos = s_textureAtlas->get(i).uv0 * s_textureAtlas->getTexture()->getSize();

		pixelData[0] = uchar(pos.x & 0xFF);
		pixelData[1] = uchar((pos.x >> 8) & 0xFF);

		pixelData[2] = uchar(pos.y & 0xFF);
		pixelData[3] = uchar((pos.y >> 8) & 0xFF);

		blockDataPixmap.setPixel(i, 0, pixelData);
	}
	s_dataTexture = Resource<Texture2D>(new Texture2D(blockDataPixmap));
	s_dataTexture->setFiltering(Texture2D::NEAREST);
}

bool BlockEntityData::canPlace(const int x, const int y, const WorldLayer layer, Terrain *terrain) const
{
	for(int y1 = y; y1 < y + m_height; y1++)
	{
		for(int x1 = x; x1 < x + m_width; x1++)
		{
			if(terrain->isBlockEntityAt(x1, y1, layer) || (layer == WORLD_LAYER_MIDDLE && terrain->isBlockAt(x1, y1, layer)))
			{
				return false;
			}
		}
	}
	return true;
}