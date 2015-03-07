#include "Entity.h"
#include "Constants.h"
#include "World/World.h"
#include "Entities/Camera.h"

Entity::Entity(const EntityID id) :
	GameObject(PRIORITY_ENTITY),
	m_id(id)
{
}

Entity::~Entity()
{
	Camera *camera = World::getCamera();
	if(camera->getTargetEntity() == this)
	{
		camera->setTargetEntity(nullptr);
	}
}