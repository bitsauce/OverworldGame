#ifndef ENTITY_H
#define ENTITY_H

#include "Config.h"
#include "Game/GameObject.h"

enum Priority;
enum EntityID;

class Entity : public GameObject
{
public:
	Entity(const EntityID id);
	virtual ~Entity();

	virtual Vector2 getPosition() const = 0;
	virtual Vector2 getSize() const = 0;
	virtual Vector2 getCenter() const { return getPosition() + getSize() * 0.5f; }

private:
	const EntityID m_id;
};

#endif // ENTITY_H