#include "EntityData.h"
#include "Arrow.h"
#include "Player.h"
#include "ItemDrop.h"
#include "Mobs/Zombie.h"

//#include "RedCurrantBush.h"
//#include "CraftingBench.h"

vector<EntityData*> EntityData::s_data(ENTITY_COUNT);

void EntityData::init(Game *)
{
	// Dynamic entities
	s_data[ENTITY_PLAYER] = new EntityData(ENTITY_PLAYER, "Player", WORLD_LAYER_MIDDLE, Player::Factory);

	/*
	s_data[ENTITY_ARROW] = new EntityData(ENTITY_ARROW, "Arrow", WORLD_LAYER_BACK, Arrow::Factory);
	s_data[ENTITY_ZOMBIE] = new EntityData(ENTITY_ZOMBIE, "Zombie", WORLD_LAYER_MIDDLE, Zombie::Factory);
	s_data[ENTITY_ITEM_DROP] = new EntityData(ENTITY_ITEM_DROP, "ItemDrop", WORLD_LAYER_MIDDLE, ItemDrop::Factory);
	*/
	// Static entitites
	//s_data[ENTITY_TORCH] = new StaticEntityData(ENTITY_TORCH, "torch", 1, 1, Torch::Factory);
}