#include "ItemData.h"
#include "Constants.h"
#include "BlockItem.h"
#include "Tools/Pickaxe.h"
#include "Tools/Axe.h"
#include "Placeable/EntitySpawnerItem.h"
#include "Weapons/Bow.h"
#include "Weapons/Ammo.h"

vector<ItemData*> ItemData::s_itemData(ITEM_COUNT);

ItemData::ItemData() :
	m_id(ITEM_NONE),
	m_name("NO ITEM"),
	m_desc(""),
	m_maxStack(0),
	m_iconTexture(nullptr)
{
}

void ItemData::set(const ItemID id, const string &name, const string &desc, const string &texture, const uint maxStack)
{
	m_id = id;
	m_name = name;
	m_desc = desc;
	m_iconTexture = ResourceManager::get<Texture2D>(texture);
	m_maxStack = maxStack;
}

void ItemData::init(OverworldGame *game)
{
	s_itemData[ITEM_NONE] = new ItemData();

	(s_itemData[ITEM_BLOCK_GRASS] = new BlockItem(game, TERRAIN_LAYER_MIDDLE, BLOCK_GRASS))->set(ITEM_BLOCK_GRASS, "Grass", "A block of grass", ":/Sprites/ItemIcons/BlockGrass.png", 255);
	(s_itemData[ITEM_BLOCK_DIRT] = new BlockItem(game, TERRAIN_LAYER_MIDDLE, BLOCK_DIRT))->set(ITEM_BLOCK_DIRT, "Dirt", "A block of dirt", ":/Sprites/ItemIcons/BlockDirt.png", 255);
	(s_itemData[ITEM_BLOCK_DIRT_BACK] = new BlockItem(game, TERRAIN_LAYER_BACK, BLOCK_DIRT_BACK))->set(ITEM_BLOCK_DIRT_BACK, "Dirt backdrop", "", ":/Sprites/ItemIcons/BlockDirtBack.png", 255);
	(s_itemData[ITEM_BLOCK_STONE] = new BlockItem(game, TERRAIN_LAYER_MIDDLE, BLOCK_STONE))->set(ITEM_BLOCK_STONE, "Stone", "A block of stone", ":/Sprites/ItemIcons/BlockStone.png", 255);
	(s_itemData[ITEM_BLOCK_OAK_WOOD] = new BlockItem(game, TERRAIN_LAYER_BACK, BLOCK_OAK_WOOD))->set(ITEM_BLOCK_OAK_WOOD, "Oak wood", "A block of oak", ":/Sprites/ItemIcons/BlockOakWood.png", 255);
	(s_itemData[ITEM_BLOCK_OAK_LEAVES] = new BlockItem(game, TERRAIN_LAYER_FRONT, BLOCK_OAK_LEAVES))->set(ITEM_BLOCK_OAK_LEAVES, "Oak leaves", "A pile of oak leaves", ":/Sprites/ItemIcons/BlockOakLeaves.png", 255);

	(s_itemData[ITEM_PICKAXE_IRON] = new Pickaxe(game))->set(ITEM_PICKAXE_IRON, "Iron Pickaxe", "An iron pickaxe", ":/Sprites/ItemIcons/IronPickaxe.png", 1);
	(s_itemData[ITEM_AXE_IRON] = new Axe(game))->set(ITEM_AXE_IRON, "Iron Axe", "An iron axe", ":/Sprites/ItemIcons/IronAxe.png", 1);

	(s_itemData[ITEM_TORCH] = new EntitySpwanerItem(game, ENTITY_TORCH))->set(ITEM_TORCH, "Torch", "Useful light source", ":/Sprites/ItemIcons/Torch.png", 255);
	(s_itemData[ITEM_CRAFTING_BENCH] = new EntitySpwanerItem(game, /*ENTITY_CRAFTING_BENCH*/ENTITY_ARROW))->set(ITEM_CRAFTING_BENCH, "Crafting Bench", "For crafting the items of your dreams", ":/Sprites/ItemIcons/CraftingBench.png", 1);
	(s_itemData[ITEM_RED_CURRANT_BUSH] = new EntitySpwanerItem(game, /*ENTITY_RED_CURRANT_BUSH*/ENTITY_ARROW))->set(ITEM_RED_CURRANT_BUSH, "Red Currant Bush", "Grows red currants", ":/Sprites/ItemIcons/Torch.png", 255);
	
	(s_itemData[ITEM_BOW_WOODEN] = new Bow(game))->set(ITEM_BOW_WOODEN, "Bow", "", ":/Sprites/ItemIcons/WoodenBow.png", 1);
	(s_itemData[ITEM_ARROW] = new Ammo(game))->set(ITEM_ARROW, "Arrow", "Needs a bow to fire", ":/Sprites/ItemIcons/Arrow.png", 255);
}