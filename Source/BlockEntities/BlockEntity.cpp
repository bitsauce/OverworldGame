#include "BlockEntity.h"
#include "World/World.h"

BlockEntity::BlockEntity(World *world, const int x, const int y, const BlockEntityData *data) :
	m_position(x, y),
	m_data(data),
	m_world(world)
{
	world->getTerrain()->placeStaticEntity(this);
	world->addStaticEntity(this);
}

BlockEntity::~BlockEntity()
{
	m_world->removeStaticEntity(this);
}