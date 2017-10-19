#pragma once

#include "Config.h"
#include "Constants.h"

class Entity;
class World;

typedef function<Entity*(World*, const Json::Value &attributes)> EntityFactory;

class EntityData
{
	friend class Overworld;
	friend class World;
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
			THROW("Could not find entity \"%s\"", name.c_str());
		}
		return itr->second;
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
	static const EntityFactory getFactory(const string &entityName);

	const EntityID m_id;
	const string m_name;
	const WorldLayer m_layer;
	const EntityFactory m_factory;

	static void init(Game *game);
	static map<string, EntityData*> s_nameToData;
	static map<EntityID, EntityData*> s_idToData;
};