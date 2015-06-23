#include "Entity.h"
#include "Constants.h"
#include "World/World.h"
#include "World/Camera.h"

Entity::Entity(World *world, const EntityID id) :
	m_world(world),
	m_id(id)
{
	m_world->addEntity(this);
}

Entity::~Entity()
{
	Camera *camera = m_world->getCamera();
	if(camera->getTargetEntity() == this)
	{
		camera->setTargetEntity(nullptr);
	}
	m_world->removeEntity(this);
}