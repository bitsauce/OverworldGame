#ifndef ENTITY_H
#define ENTITY_H

#include "Config.h"

enum Priority;
enum EntityID;
class World;

class Entity
{
public:
	Entity(World *world, const EntityID id);
	virtual ~Entity();

	const EntityID getID() const { return m_id; }

	virtual void createSaveData(FileWriter &saveData) {}
	virtual void loadSaveData(FileReader &saveData) {}

	virtual void update(const float delta) {}
	virtual void draw(SpriteBatch *spriteBatch, const float alpha) {}

protected:
	World *m_world;

private:
	const EntityID m_id;
};

#endif // ENTITY_H