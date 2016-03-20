#include "Constants.h"
#include "BlockEntityData.h"
#include "Torch.h"

vector<BlockEntityData*> BlockEntityData::s_data(BLOCK_ENTITY_COUNT);
TextureAtlas *BlockEntityData::s_atlas;

void BlockEntityData::init()
{
	s_data[BLOCK_ENTITY_NULL] = new BlockEntityData(BLOCK_ENTITY_NULL, "NULL", Pixmap(), 1, 1, function<BlockEntity*(World*, const int, const int, const BlockEntityData*)>());
	s_data[BLOCK_ENTITY_TORCH] = new BlockEntityData(BLOCK_ENTITY_TORCH, "Torch", Pixmap("Sprites/BlockEntities/LightSources/Torch.png", true), 1, 1, Torch::Factory);

	// Create block entity texture atlas
	vector<Pixmap> pixmaps;
	for(uint i = 0; i < BLOCK_ENTITY_COUNT; ++i)
	{
		pixmaps.push_back(s_data[i]->getPixmap());
	}
	s_atlas = new TextureAtlas(pixmaps);
}