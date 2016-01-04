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
	s_data[ENTITY_BACKGROUND] = new EntityData(ENTITY_BACKGROUND, "Background", WORLD_LAYER_BACK, function<Entity*(World*)>());
	s_data[ENTITY_TIME_OF_DAY] = new EntityData(ENTITY_TIME_OF_DAY, "TimeOfDay", WORLD_LAYER_BACK, function<Entity*(World*)>());
	s_data[ENTITY_CAMERA] = new EntityData(ENTITY_CAMERA, "Camera", WORLD_LAYER_BACK, function<Entity*(World*)>());
	s_data[ENTITY_TERRAIN] = new EntityData(ENTITY_TERRAIN, "Terrain", WORLD_LAYER_BACK, function<Entity*(World*)>());
	s_data[ENTITY_LIGHTING] = new EntityData(ENTITY_LIGHTING, "Lighting", WORLD_LAYER_BACK, function<Entity*(World*)>());

	/*
	s_data[ENTITY_ARROW] = new EntityData(ENTITY_ARROW, "Arrow", WORLD_LAYER_BACK, Arrow::Factory);
	s_data[ENTITY_PLAYER] = new EntityData(ENTITY_PLAYER, "Player", WORLD_LAYER_MIDDLE, Player::Factory);
	s_data[ENTITY_ZOMBIE] = new EntityData(ENTITY_ZOMBIE, "Zombie", WORLD_LAYER_MIDDLE, Zombie::Factory);
	s_data[ENTITY_ITEM_DROP] = new EntityData(ENTITY_ITEM_DROP, "ItemDrop", WORLD_LAYER_MIDDLE, ItemDrop::Factory);
	*/
	// Static entitites
	//s_data[ENTITY_TORCH] = new StaticEntityData(ENTITY_TORCH, "torch", 1, 1, Torch::Factory);
}