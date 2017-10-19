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

const EntityFactory EntityData::getFactory(const string &entityName)
{
	if(entityName == "Player") return Player::Factory;
	if(entityName == "Arrow") return Arrow::Factory;
	if(entityName == "ItemDrop") return ItemDrop::Factory;
	//if(entityName == "Stick") return Stick::Factory;
	return 0;
}

const WorldLayer getLayerFromString(const string &layerName)
{
	if (layerName == "back") return WORLD_LAYER_BACK;
	if (layerName == "middle") return WORLD_LAYER_MIDDLE;
	if (layerName == "front") return WORLD_LAYER_FRONT;
	return WORLD_LAYER_BACK;
}

void EntityData::init(Game *)
{
	LOG("Loading entity data...");

	// Load block entity data from file
	if(!util::fileExists("EntityData.json"))
	{
		THROW("EntityData.json is missing!");
	}

	// Read block entity JSON data
	Json::Value entitiesJSON;
	{
		Json::Reader reader;
		ifstream blockEntityDataFile("EntityData.json");
		if(blockEntityDataFile.is_open())
		{
			if(!reader.parse(blockEntityDataFile, entitiesJSON))
			{
				THROW("Error parsing EntityData.json!");
			}
		}
		else
		{
			THROW("Error opening EntityData.json!");
		}
	}

	// Load block entity data
	for(Json::Value::const_iterator itr = entitiesJSON.begin(); itr != entitiesJSON.end(); itr++)
	{
		const Json::Value &blockEntityJSON = *itr;
		const string name = itr.key().asString();
		const BlockID id = blockEntityJSON.get("id", -1).asInt();
		const string layer = blockEntityJSON.get("layer", -1).asString();
		const EntityFactory factory = getFactory(name);
		assert(s_idToData.find(id) == s_idToData.end());
		assert(s_nameToData.find(name) == s_nameToData.end());

		EntityData *data = new EntityData(id, name, getLayerFromString(layer), factory);
		s_nameToData[name] = data;
		s_idToData[id] = data;
	}
}