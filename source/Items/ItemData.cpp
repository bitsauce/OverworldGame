#include "ItemData.h"
#include "Game.h"

vector<ItemData*> ItemData::s_itemData(ITEM_COUNT);

ItemData::ItemData() :
	m_id(ITEM_NONE),
	m_name("NO ITEM"),
	m_desc(""),
	m_maxStack(0),
	m_icon(nullptr),
	m_singleShot(false)
{
}

void ItemData::set(const ItemID id, const string &name, const string &desc, const string &texture, const uint maxStack, const bool singleShot)
{
	m_id = id;
	m_name = name;
	m_desc = desc;
	m_icon = new Sprite(ResourceManager::get<Texture2D>(texture));
	m_maxStack = maxStack;
	m_singleShot = false;
}

class BlockItem : public ItemData
{
public:
	BlockItem(const BlockID blockID) :
		m_blockID(blockID)
	{
	}

	void use(Player *player)
	{
	}

	void draw(Player *player, SpriteBatch *spriteBatch)
	{
	}

private:

	const BlockID m_blockID;
};

void ItemData::init()
{
	(s_itemData[ITEM_OAK_BLOCK] = new BlockItem(BLOCK_WOOD))->set(ITEM_OAK_BLOCK, "Oak block", "A block of oak", ":/Sprites/ItemIcons/OakBlock.png", 255, false);
	(s_itemData[ITEM_PICKAXE_IRON] = new Pickaxe())->set(ITEM_PICKAXE_IRON, "Iron Pickaxe", "An iron pickaxe", ":/Sprites/ItemIcons/IronPickaxe.png", 1, false);
}