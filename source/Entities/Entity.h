#ifndef ENTITY_H
#define ENTITY_H

#include "Config.h"

enum Priority;
enum EntityID;
class World;

class Entity
{
public:
	Entity(World &world, const EntityID id);
	virtual ~Entity();

	virtual Vector2 getPosition() const = 0;
	virtual Vector2 getSize() const = 0;
	virtual Vector2 getCenter() const { return getPosition() + getSize() * 0.5f; }

	virtual void update(const float dt) {}
	virtual void draw(SpriteBatch *spriteBatch, const float alpha) {}

protected:
	World &m_world;

private:
	const EntityID m_id;
};

#endif // ENTITY_H