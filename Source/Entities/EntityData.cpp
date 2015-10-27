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
	// TODO:
	// 1) Separate StaticEntities and DynamicEntities
	// 2) Rename StaticEntitiy -> BlockEntity
	// 3) Rename DyanicEntity -> Entity (maybe)
	// 4) Static Entities should be contained in a vertex buffer handled by the chunk loader

	// Dynamic entities
	s_data[ENTITY_ARROW] = new DynamicEntityData(ENTITY_ARROW, "Arrow", Arrow::Factory);
	s_data[ENTITY_PLAYER] = new DynamicEntityData(ENTITY_PLAYER, "Player", Player::Factory);
	s_data[ENTITY_ZOMBIE] = new DynamicEntityData(ENTITY_ZOMBIE, "Zombie", Zombie::Factory);
	s_data[ENTITY_ITEM_DROP] = new DynamicEntityData(ENTITY_ITEM_DROP, "ItemDrop", ItemDrop::Factory);

	// Static entitites
	s_data[ENTITY_TORCH] = new StaticEntityData(ENTITY_TORCH, "torch", 1, 1, Torch::Factory);
}