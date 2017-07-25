#pragma once

#include "Config.h"
#include "Constants.h"

class Entity;

typedef function<Entity*(const Json::Value&)> EntityFactory;

const WorldLayer getLayerFromString(const string &layerName)
{
	if (layerName == "back") return WORLD_LAYER_BACK;
	if (layerName == "middle") return WORLD_LAYER_MIDDLE;
	if (layerName == "front") return WORLD_LAYER_FRONT;
	return WORLD_LAYER_BACK;
}

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
		if(itr != s_idToData.end())
		{
			return itr->second;
		}
		return 0;
	}

	static EntityData *GetByName(const string &name)
	{
		map<string, EntityData*>::iterator itr = s_nameToData.find(name);
		if(itr != s_nameToData.end())
		{
			return itr->second;
		}
		return 0;
	}

	static Entity *Create(const EntityID id, const Json::Value &attributes)
	{
		EntityData *data = Get(id);
		if(!data) THROW("Cannot create entity with id=%i. No entity with that ID exists", id);
		return data->m_factory(attributes);
	}

	static Entity *CreateByName(const string &name, const Json::Value &attributes)
	{
		EntityData *data = GetByName(name);
		if(!data) THROW("Cannot create entity '%s'. No entity with that name exists", name.c_str());
		return data->m_factory(attributes);
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