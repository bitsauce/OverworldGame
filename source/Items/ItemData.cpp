#include "ItemData.h"
#include "Constants.h"
#include "BlockItem.h"
#include "Tools/Pickaxe.h"
#include "Placeable/ThingItem.h"
#include "Weapons/Bow.h"

vector<ItemData*> ItemData::s_itemData(ITEM_COUNT);

ItemData::ItemData() :
	m_id(ITEM_NONE),
	m_name("NO ITEM"),
	m_desc(""),
	m_maxStack(0),
	m_iconTexture(nullptr),
	m_singleShot(false)
{
}

void ItemData::set(const ItemID id, const string &name, const string &desc, const string &texture, const uint maxStack, const bool singleShot)
{
	m_id = id;
	m_name = name;
	m_desc = desc;
	m_iconTexture = ResourceManager::get<Texture2D>(texture);
	m_maxStack = maxStack;
	m_singleShot = singleShot;
}

void ItemData::init(Game *game)
{
	(s_itemData[ITEM_GRASS_BLOCK] = new BlockItem(game, TERRAIN_LAYER_MIDDLE, BLOCK_GRASS))->set(ITEM_GRASS_BLOCK, "Dirt", "A block of dirt", ":/Sprites/ItemIcons/GrassBlock.png", 255, false);
	(s_itemData[ITEM_DIRT_BACK_BLOCK] = new BlockItem(game, TERRAIN_LAYER_BACK, BLOCK_DIRT_BACK))->set(ITEM_DIRT_BACK_BLOCK, "Dirt backdrop", "", ":/Sprites/ItemIcons/DirtBackBlock.png", 255, false);
	(s_itemData[ITEM_STONE_BLOCK] = new BlockItem(game, TERRAIN_LAYER_MIDDLE, BLOCK_STONE))->set(ITEM_STONE_BLOCK, "Cobblestone", "A block of stone", ":/Sprites/ItemIcons/StoneBlock.png", 255, false);
	(s_itemData[ITEM_PICKAXE_IRON] = new Pickaxe(game))->set(ITEM_PICKAXE_IRON, "Iron Pickaxe", "An iron pickaxe", ":/Sprites/ItemIcons/IronPickaxe.png", 1, false);

	(s_itemData[ITEM_TORCH] = new ThingItem(game, BLOCK_ENTITY_TORCH))->set(ITEM_TORCH, "Torch", "Useful light source", ":/Sprites/ItemIcons/Torch.png", 255, false);
	(s_itemData[ITEM_CRAFTING_BENCH] = new ThingItem(game, BLOCK_ENTITY_CRAFTING_BENCH))->set(ITEM_CRAFTING_BENCH, "Crafting Bench", "For crafting the items of your dreams", ":/Sprites/ItemIcons/CraftingBench.png", 1, false);
	(s_itemData[ITEM_RED_CURRANT_BUSH] = new ThingItem(game, BLOCK_ENTITY_RED_CURRANT_BUSH))->set(ITEM_RED_CURRANT_BUSH, "Red Currant Bush", "Grows red currants", ":/Sprites/ItemIcons/Torch.png", 255, false);

	(s_itemData[ITEM_BOW_WOODEN] = new Bow(game))->set(ITEM_BOW_WOODEN, "Bow", "", ":/Sprites/ItemIcons/WoodenBow.png", 1, true);
}