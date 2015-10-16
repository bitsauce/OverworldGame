#include "EntityData.h"
#include "Dynamic/Arrow.h"
#include "Dynamic/Player.h"
#include "Dynamic/ItemDrop.h"
#include "Dynamic/Mobs/Zombie.h"

#include "Static/Torch.h"

//#include "RedCurrantBush.h"
//#include "CraftingBench.h"

vector<EntityData*> EntityData::s_data(ENTITY_COUNT);

void EntityData::init(Game *game)
{
	// TODO: Add a specific descriptor for each type of entity.
	//
	// This means to add something like
	// DynamicDesc, StaticDesc and ControllerDesc
	// which contain specific information like
	// the width and height in the case of StaticDesc
	// (which is usefull for placing static entities in chunk grids)
	/*s_factories[ENTITY_ARROW] = Arrow::Factory;
	s_factories[ENTITY_PLAYER] = Player::Factory;
	s_factories[ENTITY_ZOMBIE] = Zombie::Factory;
	s_factories[ENTITY_ITEM_DROP] = ItemDrop::Factory;*/

	//s_data[ENTITY_TORCH] = new StaticEntityData(ENTITY_TORCH, 1, 1, Torch::Factory);
}