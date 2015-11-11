#ifndef ENTITY_H
#define ENTITY_H

#include "Config.h"

enum Priority;
enum EntityID;
class World;
class EntityData;

class Entity
{
public:
	Entity(World *world, const EntityID id);
	virtual ~Entity();

	const EntityData *getData() const { return m_data; }

	virtual void createSaveData(FileWriter &saveData) {}
	virtual void loadSaveData(FileReader &saveData) {}

	virtual void update(const float delta) {}
	virtual void draw(SpriteBatch *spriteBatch, const float alpha) {}

protected:
	const EntityData * const m_data;
	World * const m_world;
};

#endif // ENTITY_H