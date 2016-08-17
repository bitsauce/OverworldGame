#include "ItemData.h"
#include "Constants.h"
#include "Tools/Pickaxe.h"
#include "Tools/Axe.h"
#include "Placeable/BlockItem.h"
#include "Placeable/BlockEntityItem.h"
#include "Placeable/TorchItem.h"
#include "Weapons/Bow.h"
#include "Weapons/Ammo.h"

map<ItemID, ItemData*> ItemData::s_data;

ItemData::ItemData(const ItemDataDesc *itemDataDesc) :
	m_id(itemDataDesc->id),
	m_name(itemDataDesc->name),
	m_desc(itemDataDesc->desc),
	m_maxStack(itemDataDesc->maxStack),
	m_iconTexture(Resource<Texture2D>(itemDataDesc->icon))
{
}

ItemData *ItemData::getByName(const string &name)
{
	for(map<ItemID, ItemData*>::iterator itr = s_data.begin(); itr != s_data.end(); ++itr)
	{
		if(itr->second && itr->second->m_name == name)
		{
			return itr->second;
		}
	}
	return 0;
}

void ItemData::init(OverworldGame *game)
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

				ItemDataDesc itemDesc = { util::strToInt(id->GetText()), name->GetText(), invName->GetText(), desc->GetText(), type->GetText(), icon->GetText(), (uint)util::strToInt(maxStack->GetText()), userDataMap };
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

	for(int i = 0; i < itemDataDescs.size(); i++)
	{
		ItemDataDesc *itemDataDesc = &itemDataDescs[i];

		ItemData *itemData = 0;
		if(itemDataDesc->type == "block_item")
		{
			itemData = new BlockItem(game, itemDataDesc);
		}
		else if(itemDataDesc->type == "axe")
		{
			itemData = new Axe(game, itemDataDesc);
		}
		else if(itemDataDesc->type == "pickaxe")
		{
			itemData = new Pickaxe(game, itemDataDesc);
		}
		else if(itemDataDesc->type == "torch")
		{
			itemData = new TorchItem(game, itemDataDesc);
		}
		else if(itemDataDesc->type == "block_entity_item")
		{
			itemData = new BlockEntityItem(game, itemDataDesc);
		}
		else if(itemDataDesc->type == "ammo")
		{
			itemData = new Ammo(itemDataDesc);
		}
		else if(itemDataDesc->type == "bow")
		{
			itemData = new Bow(game, itemDataDesc);
		}
		else
		{
			LOG("Unknown item type '%s'", itemDataDesc->type.c_str());
		}
		s_data[itemDataDesc->id] = itemData;
	}

#ifdef POTET
	s_data[ITEM_NONE] = new ItemData();

	(s_data[ITEM_BLOCK_GRASS] = new BlockItem(game, WORLD_LAYER_MIDDLE, BLOCK_GRASS))->set(ITEM_BLOCK_GRASS, "Grass", "A block of grass", "Sprites/ItemIcons/BlockGrass", 255);
	(s_data[ITEM_BLOCK_DIRT] = new BlockItem(game, WORLD_LAYER_MIDDLE, BLOCK_DIRT))->set(ITEM_BLOCK_DIRT, "Dirt", "A block of dirt", "Sprites/ItemIcons/BlockDirt", 255);
	(s_data[ITEM_BLOCK_DIRT_BACK] = new BlockItem(game, WORLD_LAYER_BACK, BLOCK_DIRT_BACK))->set(ITEM_BLOCK_DIRT_BACK, "Dirt backdrop", "", "Sprites/ItemIcons/BlockDirtBack", 255);
	(s_data[ITEM_BLOCK_STONE] = new BlockItem(game, WORLD_LAYER_MIDDLE, BLOCK_STONE))->set(ITEM_BLOCK_STONE, "Stone", "A block of stone", "Sprites/ItemIcons/BlockStone", 255);
	(s_data[ITEM_BLOCK_OAK_WOOD] = new BlockItem(game, WORLD_LAYER_BACK, BLOCK_OAK_WOOD))->set(ITEM_BLOCK_OAK_WOOD, "Oak wood", "A block of oak", "Sprites/ItemIcons/BlockOakWood", 255);
	(s_data[ITEM_BLOCK_OAK_LEAVES] = new BlockItem(game, WORLD_LAYER_FRONT, BLOCK_OAK_LEAVES))->set(ITEM_BLOCK_OAK_LEAVES, "Oak leaves", "A pile of oak leaves", "Sprites/ItemIcons/BlockOakLeaves", 255);

	(s_data[ITEM_PICKAXE_IRON] = new Pickaxe(game))->set(ITEM_PICKAXE_IRON, "Iron Pickaxe", "An iron pickaxe. Used for mining ores.", "Sprites/ItemIcons/Pickaxe_Iron", 1);
	(s_data[ITEM_AXE_IRON] = new Axe(game))->set(ITEM_AXE_IRON, "Iron Axe", "An iron axe. Used for chopping down trees.", "Sprites/ItemIcons/Axe_Iron", 1);

	(s_data[ITEM_TORCH] = new BlockEntityItem(game, BLOCK_ENTITY_TORCH))->set(ITEM_TORCH, "Torch", "Useful light source during night or when exploring dark areas.", "Sprites/ItemIcons/Torch", 255);
	(s_data[ITEM_CRAFTING_BENCH] = new BlockEntityItem(game, /*ENTITY_CRAFTING_BENCH*/BLOCK_ENTITY_TORCH))->set(ITEM_CRAFTING_BENCH, "Crafting Bench", "For crafting the items of your dreams", "Sprites/ItemIcons/CraftingBench", 1);
	(s_data[ITEM_RED_CURRANT_BUSH] = new BlockEntityItem(game, /*ENTITY_RED_CURRANT_BUSH*/BLOCK_ENTITY_TORCH))->set(ITEM_RED_CURRANT_BUSH, "Red Currant Bush", "Grows red currants", "Sprites/ItemIcons/Torch", 255);
	
	(s_data[ITEM_BOW_WOODEN] = new Bow(game))->set(ITEM_BOW_WOODEN, "Bow", "Ranged weapon. Charge up to fire arrows further. Require arrows to use.", "Sprites/ItemIcons/Bow", 1);
	(s_data[ITEM_ARROW] = new Ammo(game))->set(ITEM_ARROW, "Arrow", "Common projectile for a bow.", "Sprites/ItemIcons/Arrow", 255);
#endif
}