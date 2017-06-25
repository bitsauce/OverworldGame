#include "EntityData.h"
#include "Arrow.h"
#include "Player.h"
#include "ItemDrop.h"
#include "DebugLight.h"
#include "Mobs/Zombie.h"
#include "Stick.h"

//#include "RedCurrantBush.h"
//#include "CraftingBench.h"

map<string, EntityData*> EntityData::s_nameToData;
map<EntityID, EntityData*> EntityData::s_idToData;

struct EntityDataDesc
{
	const EntityID id;
	const string name;
	const WorldLayer layer;
	const EntityFactory factory;
};

const WorldLayer getLayerFromString(const string &layerName)
{
	if(layerName == "back") return WORLD_LAYER_BACK;
	if(layerName == "middle") return WORLD_LAYER_MIDDLE;
	if(layerName == "front") return WORLD_LAYER_FRONT;
	return WORLD_LAYER_BACK;
}

const EntityFactory getFactory(const string &entityName)
{
	if(entityName == "Player") return Player::Factory;
	if(entityName == "Arrow") return Arrow::Factory;
	if(entityName == "ItemDrop") return ItemDrop::Factory;
	return 0;
}

void EntityData::init(Game *)
{
	LOG("Loading entity data...");

	// Load entity data from file
	vector<EntityDataDesc> entityDataDesc;

	if(util::fileExists("EntityData.xml"))
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile("EntityData.xml");

		// Get root node
		tinyxml2::XMLNode *entityNode = doc.FirstChildElement();
		if(!entityNode)
		{
			LOG("EntityData.xml has no root node!");
			return;
		}

		// For each block node
		entityNode = entityNode->FirstChildElement();
		while(entityNode)
		{
			// For each block entry
			tinyxml2::XMLElement *id = entityNode->FirstChildElement("id");
			tinyxml2::XMLElement *name = entityNode->FirstChildElement("name");
			tinyxml2::XMLElement *layer = entityNode->FirstChildElement("layer");

			if(id && name && layer)
			{
				EntityDataDesc desc = { (EntityID)util::strToInt(id->GetText()), name->GetText(), getLayerFromString(layer->GetText()), getFactory(name->GetText()) };
				entityDataDesc.push_back(desc);
			}
			else
			{
				if(name)
				{
					LOG("Loading entity '%s' failed", name->GetText());
				}
				else if(id)
				{
					LOG("Loading entity (id='%s') failed", id->GetText());
				}
				else
				{
					LOG("Loading entity (nr='%i') failed", entityDataDesc.size());
				}
			}

			// Next resource
			entityNode = entityNode->NextSibling();
		}
	}
	else
	{
		THROW("EntityData.xml is missing!");
	}

	// Create all entity data
	for(EntityDataDesc &entityData : entityDataDesc)
	{
		EntityData *data = new EntityData(entityData.id, entityData.name, entityData.layer, entityData.factory);
		s_nameToData[entityData.name] = data;
		s_idToData[entityData.id] = data;
	}
}