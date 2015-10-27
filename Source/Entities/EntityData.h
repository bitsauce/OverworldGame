#pragma once

#include "Config.h"
#include "Constants.h"

class World;
class Entity;
class DynamicEntity;
class StaticEntity;

enum EntityType
{
	DYNAMIC_ENTITY,
	STATIC_ENTITY,
	CONTROLLER_ENTITY
};

class EntityData
{
	friend class OverworldGame;
public:
	EntityData(const EntityID id, const EntityType type, const string &name) :
		m_id(id),
		m_type(type),
		m_name(name)
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

	const EntityType getType() const
	{
		return m_type;
	}

private:
	const EntityID m_id;
	const EntityType m_type;
	const string m_name;

	static void init(Game *game);
	static vector<EntityData*> s_data;
};

class DynamicEntityData : public EntityData
{
public:
	DynamicEntityData(const EntityID id, const string &name, function<DynamicEntity * (World *)> factory) :
		EntityData(id, DYNAMIC_ENTITY, name),
		m_factory(factory)
	{
	}

	DynamicEntity *create(World *world)
	{
		if(!m_factory)
		{
			THROW("Entity (id=%i) has no factory", getID());
		}
		return m_factory(world);
	}

private:
	function<DynamicEntity *(World *)> m_factory;
};

class StaticEntityData : public EntityData
{
public:
	StaticEntityData(const EntityID id, const string &name, const int width, const int height, function<StaticEntity * (World *, const int, const int)> factory) :
		EntityData(id, STATIC_ENTITY, name),
		m_size(width, height),
		m_factory(factory)
	{
	}

	StaticEntity *create(World * world, const int x, const int y)
	{
		if(!m_factory)
		{
			THROW("Entity (id=%i) has no factory", getID());
		}
		return m_factory(world, x, y);
	}

	Vector2i getSize() const
	{
		return m_size;
	}

private:
	const Vector2i m_size;
	function<StaticEntity * (World *, const int, const int)> m_factory;
};