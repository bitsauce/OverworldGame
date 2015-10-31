#include "BlockEntity.h"
#include "World/World.h"

BlockEntity::BlockEntity(World *world, const int x, const int y, const BlockEntityData *data) :
	m_position(x, y)
{
	world->getTerrain()->placeStaticEntity(this);
}