#pragma once

#include "Config.h"
#include "Constants.h"

class Entity;

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
	static Entity* Create(Game *game, const EntityID id)
	{
		return s_factories[id] ? s_factories[id](game) : nullptr;
	}

	//const EntityData getID() const
	//{
	//	return m_id;
	//}

private:
	//const EntityID m_id;
	//const function<void()> m_factory;
	const EntityType m_type;

	static void init(Game *game);
	//static vector<EntityData*> s_data;
	static vector<function<Entity*(Game *game)>> s_factories;
};