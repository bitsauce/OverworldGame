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
	friend class Game;
public:
	EntityData(const EntityID id, const EntityType type) :
		m_id(id),
		m_type(type)
	{
	}

	static EntityData * Get(const EntityID id)
	{
		if(!s_data[id])
		{
			THROW("Entity (id=%i) has no EntityData", id);
		}
		return s_data[id];
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

	static void init(Game * game);
	static vector<EntityData*> s_data;
};

class DynamicEntityData : public EntityData
{
public:
	DynamicEntityData(const EntityID id, function<DynamicEntity * (World *)> factory) :
		EntityData(id, DYNAMIC_ENTITY),
		m_factory(factory)
	{
	}

	DynamicEntity * Create(World *world)
	{
		if(!m_factory)
		{
			THROW("Entity (id=%i) has no factory", getID());
		}
		return m_factory(world);
	}

private:
	function<DynamicEntity * (World *)> m_factory;
};

class StaticEntityData : public EntityData
{
public:
	StaticEntityData(const EntityID id, const int width, const int height, function<StaticEntity * (World *, const int, const int)> factory) :
		EntityData(id, STATIC_ENTITY),
		m_size(width, height),
		m_factory(factory)
	{
	}

	StaticEntity * Create(World * world, const int x, const int y)
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