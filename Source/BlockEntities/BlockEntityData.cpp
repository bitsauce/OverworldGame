#include "Constants.h"
#include "BlockEntityData.h"

vector<BlockEntityData*> BlockEntityData::s_data(BLOCK_ENTITY_COUNT);

void BlockEntityData::init()
{
	s_data[BLOCK_ENTITY_TORCH] = new BlockEntityData(BLOCK_ENTITY_TORCH, "Torch", 1, 1, Torch::Factory);
}