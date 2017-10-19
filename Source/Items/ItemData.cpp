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

function<Item*(World*, Pawn*)> getItemFactory(const string &itemName)
{
	if(itemName == "Bow") return Bow::Factory;
	if(itemName == "IronPickaxe") return Pickaxe::Factory;
	return 0;
}

ItemData::ItemData(const ItemDataDesc *itemDataDesc) :
	m_id(itemDataDesc->id),
	m_name(itemDataDesc->name),
	m_desc(itemDataDesc->desc),
	m_maxStack(itemDataDesc->maxStack),
	m_iconTexture(Resource<Texture2D>(itemDataDesc->icon)),
	m_factory(itemDataDesc->factory)
{
}

void ItemData::init(Overworld *game)
{
	LOG("Loading item data...");

	// Load block data from file
	vector<ItemDataDesc> itemDataDescs;
	if(util::fileExists("ItemData.xml"))
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile("ItemData.xml");

		// Get root node
		tinyxml2::XMLNode *blockNode = doc.FirstChildElement();
		if(!blockNode)
		{
			LOG("ItemData.xml has no root node!");
			return;
		}

		// For each block node
		blockNode = blockNode->FirstChildElement();
		while(blockNode)
		{
			// For each block entry
			tinyxml2::XMLElement *id = blockNode->FirstChildElement("id");
			tinyxml2::XMLElement *name = blockNode->FirstChildElement("name");
			tinyxml2::XMLElement *invName = blockNode->FirstChildElement("inventoryName");
			tinyxml2::XMLElement *desc = blockNode->FirstChildElement("description");
			tinyxml2::XMLElement *type = blockNode->FirstChildElement("type");
			tinyxml2::XMLElement *icon = blockNode->FirstChildElement("icon");
			tinyxml2::XMLElement *maxStack = blockNode->FirstChildElement("maxStack");

			if(id && name && invName && desc && type && icon && maxStack)
			{
				// Get user data if any
				tinyxml2::XMLElement *userData = blockNode->FirstChildElement("userData");
				map<string, string> userDataMap;
				if(userData)
				{
					userData = userData->FirstChildElement();
					while(userData)
					{
						userDataMap[userData->Name()] = userData->GetText();
						userData = userData->NextSiblingElement();
					}
				}

				ItemDataDesc itemDesc = { util::strToInt(id->GetText()), name->GetText(), invName->GetText(), desc->GetText(), type->GetText(), icon->GetText(), (uint)util::strToInt(maxStack->GetText()), getItemFactory(name->GetText()), userDataMap };
				itemDataDescs.push_back(itemDesc);
			}
			else
			{
				if(name)
				{
					LOG("Loading item '%s' failed", name->GetText());
				}
				else if(id)
				{
					LOG("Loading item (id='%s') failed", id->GetText());
				}
				else
				{
					LOG("Loading item (nr='%i') failed", itemDataDescs.size());
				}
			}

			// Next resource
			blockNode = blockNode->NextSibling();
}
	}
	else
	{
		THROW("ItemData.xml is missing!");
	}

	s_itemDataVector.resize(itemDataDescs.size());

	for(int i = 0; i < itemDataDescs.size(); i++)
	{
		ItemDataDesc *desc = &itemDataDescs[i];
		s_itemDataVector[desc->id] = s_nameToData[desc->name] = new ItemData(desc);
	}
}