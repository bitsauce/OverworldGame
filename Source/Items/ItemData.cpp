#include "ItemData.h"
#include "Constants.h"
#include "Tools/Pickaxe.h"
#include "Tools/Axe.h"
#include "Placeable/BlockItem.h"
#include "Placeable/BlockEntityItem.h"
#include "Placeable/TorchItem.h"
#include "Weapons/Bow.h"
#include "Weapons/Ammo.h"

vector<ItemData*> ItemData::s_itemDataVector;
map<string, ItemData*> ItemData::s_nameToData;

ItemFactory getItemFactory(const string &itemClass)
{
	if(itemClass == "Bow") return Bow::Factory;
	if(itemClass == "Pickaxe") return Pickaxe::Factory;
	if(itemClass == "BlockItem") return BlockItem::Factory;
	return 0;
}

ItemData::ItemData(const ItemDataDesc *itemDataDesc) :
	m_id(itemDataDesc->id),
	m_name(itemDataDesc->name),
	m_desc(itemDataDesc->desc),
	m_maxStack(itemDataDesc->maxStack),
	m_iconTexture(Resource<Texture2D>(itemDataDesc->icon)),
	m_factory(itemDataDesc->factory),
	m_attributes(itemDataDesc->attributes)
{
}

#define JSON_GET_INT(subkey) \
	if(!itemJSON.isMember(#subkey)) THROW("\"%s\" missing \"%s\"", key, #subkey); \
	const int subkey = itemJSON[#subkey].asInt();

#define JSON_GET_STR(subkey) \
	if(!itemJSON.isMember(#subkey)) THROW("\"%s\" missing \"%s\"", key, #subkey); \
	const string subkey = itemJSON[#subkey].asString();

void ItemData::init(Overworld *game)
{
	LOG("Loading item data...");

	// Load item data from file
	if(!util::fileExists("ItemData.json"))
	{
		THROW("ItemData.json is missing!");
	}

	// Read block item JSON data
	Json::Value itemsJSON;
	{
		Json::Reader reader;
		ifstream dataFile("ItemData.json");
		if(dataFile.is_open())
		{
			if(!reader.parse(dataFile, itemsJSON))
			{
				THROW("Error parsing ItemData.json!");
			}
		}
		else
		{
			THROW("Error opening ItemData.json!");
		}
	}

	// Load item data
	vector<ItemDataDesc> itemDataDescs;
	for(Json::Value::const_iterator itr = itemsJSON.begin(); itr != itemsJSON.end(); itr++)
	{
		const Json::Value &itemJSON = *itr;
		const string key = itr.key().asString();

		JSON_GET_INT(id);
		JSON_GET_STR(name);
		JSON_GET_STR(description);
		if(!itemJSON.isMember("class")) THROW("\"%s\" missing \"%s\"", key, "class");
		const string itemClass = itemJSON["class"].asString();
		JSON_GET_STR(icon);
		JSON_GET_INT(maxStack);

		ItemDataDesc desc = { id, key, name, description, icon, maxStack, getItemFactory(itemClass), itemJSON["attributes"] };
		itemDataDescs.push_back(desc);
	}

	s_itemDataVector.resize(itemDataDescs.size());

	for(int i = 0; i < itemDataDescs.size(); i++)
	{
		ItemDataDesc *desc = &itemDataDescs[i];
		s_itemDataVector[desc->id] = s_nameToData[desc->name] = new ItemData(desc);
	}
}