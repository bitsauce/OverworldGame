#include "Constants.h"
#include "BlockEntityData.h"

vector<BlockEntityData*> BlockEntityData::s_data(BLOCK_ENTITY_COUNT);
TextureAtlas *BlockEntityData::s_atlas;

void BlockEntityData::init()
{
	// TODO: Make some shader for drawing animated block entities

	s_data[BLOCK_ENTITY_NULL] = new BlockEntityData(BLOCK_ENTITY_NULL, "NULL", ":/Sprites/BlockEntities/LightSources/Torch.png", 1, 1, 0);
	s_data[BLOCK_ENTITY_TORCH] = new BlockEntityData(BLOCK_ENTITY_TORCH, "Torch", ":/Sprites/BlockEntities/LightSources/Torch.png", 1, 1, Torch::Factory);

	// Create block texture atlas
	vector<Texture2DPtr> textures;
	for(uint i = 0; i < BLOCK_COUNT; ++i)
	{
		textures.push_back(s_data[i]->m_texture);
	}
	s_atlas = new TextureAtlas(textures);
}