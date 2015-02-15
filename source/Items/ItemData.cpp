#include "Game.h"

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
	m_singleShot = false;
}

void ItemData::init()
{
	(s_itemData[ITEM_GRASS_BLOCK] = new BlockItem(TERRAIN_LAYER_MIDDLE, BLOCK_GRASS))->set(ITEM_GRASS_BLOCK, "Dirt block", "A block of dirt", ":/Sprites/ItemIcons/DirtBlock.png", 255, false);
	(s_itemData[ITEM_PICKAXE_IRON] = new Pickaxe())->set(ITEM_PICKAXE_IRON, "Iron Pickaxe", "An iron pickaxe", ":/Sprites/ItemIcons/IronPickaxe.png", 1, false);
	(s_itemData[ITEM_TORCH] = new BlockEntityItem(BLOCK_ENTITY_RED_CURRANT_BUSH))->set(ITEM_TORCH, "Red currant bush", "Grows red currants", ":/Sprites/ItemIcons/Torch.png", 255, false);
}