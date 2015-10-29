#pragma once

#include "Config.h"
#include "Constants.h"

class World;
class Entity;
class DynamicEntity;
class StaticEntity;

class EntityData
{
	friend class OverworldGame;
public:
	EntityData(const EntityID id, const string &name, const function<Entity*(World*)> factory) :
		m_id(id),
		m_name(name),
		m_factory(factory)
	{
	}

	static EntityData *get(const EntityID id)
	{
		if(!s_data[id])
		{
			THROW("Entity (id=%i) has no EntityData", id);
		}
		return s_data[id];
	}

	static EntityData *getByName(const string &name)
	{
		for(int i = 0; i < ENTITY_COUNT; ++i)
		{
			if(s_data[i] && s_data[i]->m_name == name)
			{
				return s_data[i];
			}
		}
		return 0;
	}

	const EntityID getID() const
	{
		return m_id;
	}

	Entity *create(World *world)
	{
		if(!m_factory)
		{
			THROW("Entity (id=%i) has no factory", getID());
		}
		return m_factory(world);
	}

private:
	const EntityID m_id;
	const string m_name;
	const function<Entity*(World*)> m_factory;

	static void init(Game *game);
	static vector<EntityData*> s_data;
};