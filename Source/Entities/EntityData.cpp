#include "EntityData.h"
#include "Arrow.h"
#include "Player.h"
#include "ItemDrop.h"
#include "DebugLight.h"
#include "Mobs/Zombie.h"
#include "Stick.h"

//#include "RedCurrantBush.h"
//#include "CraftingBench.h"

vector<EntityData*> EntityData::s_data(ENTITY_COUNT);

void EntityData::init(Game *)
{
	// Dynamic entities
	s_data[ENTITY_PLAYER] = new EntityData(ENTITY_PLAYER, "Player", WORLD_LAYER_MIDDLE, Player::Factory);
	s_data[ENTITY_ARROW] = new EntityData(ENTITY_ARROW, "Arrow", WORLD_LAYER_BACK, Arrow::Factory);

	s_data[ENTITY_ZOMBIE] = new EntityData(ENTITY_ZOMBIE, "Zombie", WORLD_LAYER_MIDDLE, Zombie::Factory);
	s_data[ENTITY_ITEM_DROP] = new EntityData(ENTITY_ITEM_DROP, "ItemDrop", WORLD_LAYER_MIDDLE, ItemDrop::Factory);

	s_data[ENTITY_DEBUG_POINTLIGHT] = new EntityData(ENTITY_DEBUG_POINTLIGHT, "Debug_Pointlight", WORLD_LAYER_MIDDLE, DebugPointlight::Factory);

	s_data[ENTITY_STICK] = new EntityData(ENTITY_STICK, "Stick", WORLD_LAYER_MIDDLE, Stick::Factory);
}