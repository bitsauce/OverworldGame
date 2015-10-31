#include "EntityData.h"
#include "Dynamic/Arrow.h"
#include "Dynamic/Player.h"
#include "Dynamic/ItemDrop.h"
#include "Dynamic/Mobs/Zombie.h"

//#include "RedCurrantBush.h"
//#include "CraftingBench.h"

vector<EntityData*> EntityData::s_data(ENTITY_COUNT);

void EntityData::init(Game *)
{
	// Dynamic entities
	s_data[ENTITY_ARROW] = new EntityData(ENTITY_ARROW, "Arrow", Arrow::Factory);
	s_data[ENTITY_PLAYER] = new EntityData(ENTITY_PLAYER, "Player", Player::Factory);
	s_data[ENTITY_ZOMBIE] = new EntityData(ENTITY_ZOMBIE, "Zombie", Zombie::Factory);
	s_data[ENTITY_ITEM_DROP] = new EntityData(ENTITY_ITEM_DROP, "ItemDrop", ItemDrop::Factory);

	// Static entitites
	//s_data[ENTITY_TORCH] = new StaticEntityData(ENTITY_TORCH, "torch", 1, 1, Torch::Factory);
}