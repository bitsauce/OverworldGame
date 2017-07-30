#pragma once

#include "Config.h"
#include "Constants.h"

class Entity;

typedef function<Entity*(const Json::Value&)> EntityFactory;

class EntityData
{
	friend class OverworldGame;
public:
	EntityData(const EntityID id, const string &name, const WorldLayer layer, const EntityFactory factory) :
		m_id(id),
		m_name(name),
		m_layer(layer),
		m_factory(factory)
	{
		if(!m_factory) THROW("Cannot create EntityData for %s. Missing factory function", name.c_str());
	}

	static EntityData *Get(const EntityID id)
	{
		map<EntityID, EntityData*>::iterator itr = s_idToData.find(id);
		if(itr == s_idToData.end())
		{
			THROW("Could not find entity with id=%i", id);
		}
		return itr->second;
	}

	static EntityData *GetByName(const string &name)
	{
		map<string, EntityData*>::iterator itr = s_nameToData.find(name);
		if(itr == s_nameToData.end())
		{
			THROW("Could not find entity with name=%s", name.c_str());
		}
		return itr->second;
	}

	static Entity *Create(const EntityID id, const Json::Value &attributes)
	{
		return Get(id)->m_factory(attributes);
	}

	static Entity *CreateByName(const string &name, const Json::Value &attributes)
	{
		return GetByName(name)->m_factory(attributes);
	}

	EntityID getID() const
	{
		return m_id;
	}

	string getName() const
	{
		return m_name;
	}

	WorldLayer getLayer() const
	{
		return m_layer;
	}

private:
	const EntityID m_id;
	const string m_name;
	const WorldLayer m_layer;
	const EntityFactory m_factory;

	static void init(Game *game);
	static map<string, EntityData*> s_nameToData;
	static map<EntityID, EntityData*> s_idToData;
};