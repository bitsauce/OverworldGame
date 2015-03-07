#include "Entity.h"
#include "Constants.h"
#include "World/World.h"
#include "Entities/Camera.h"

Entity::Entity(World &world, const EntityID id) :
	GameObject(PRIORITY_ENTITY),
	m_world(world),
	m_id(id)
{
	m_world.addEntity(this);
}

Entity::~Entity()
{
	Camera *camera = m_world.getCamera();
	if(camera->getTargetEntity() == this)
	{
		camera->setTargetEntity(nullptr);
	}
	m_world.removeEntity(this);
}