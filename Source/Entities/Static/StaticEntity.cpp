#include "StaticEntity.h"
#include "World/World.h"

StaticEntity::StaticEntity(World * world, int x, int y, int w, int h, const EntityID id) :
	Entity(world, id),
	m_refCount(0)
{
	m_position.set(x, y);
	world->getTerrain()->placeStaticEntity(this);
}