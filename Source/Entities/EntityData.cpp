#include "EntityData.h"
#include "Dynamic/Arrow.h"
#include "Dynamic/Player.h"
#include "Dynamic/ItemDrop.h"
#include "Dynamic/Mobs/Zombie.h"

#include "Static/Torch.h"

//#include "RedCurrantBush.h"
//#include "CraftingBench.h"

vector<EntityData*> EntityData::s_data(ENTITY_COUNT);

void EntityData::init(Game *)
{
	// TODO: Add a specific descriptor for each type of entity.
	//
	// This means to add something like
	// DynamicDesc, StaticDesc and ControllerDesc
	// which contain specific information like
	// the width and height in the case of StaticDesc
	// (which is usefull for placing static entities in chunk grids)

	// Dynamic entities
	s_data[ENTITY_ARROW] = new DynamicEntityData(ENTITY_ARROW, Arrow::Factory);
	s_data[ENTITY_PLAYER] = new DynamicEntityData(ENTITY_PLAYER, Player::Factory);
	s_data[ENTITY_ZOMBIE] = new DynamicEntityData(ENTITY_ZOMBIE, Zombie::Factory);
	s_data[ENTITY_ITEM_DROP] = new DynamicEntityData(ENTITY_ITEM_DROP, ItemDrop::Factory);

	// Static entitites
	s_data[ENTITY_TORCH] = new StaticEntityData(ENTITY_TORCH, 1, 1, Torch::Factory);
}