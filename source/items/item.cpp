#include "Item.h"
#include "Game.h"

vector<ItemData*> ItemData::s_itemData(ITEM_COUNT);

class BlockItem : public ItemData
{
public:
	BlockItem(const ItemID itemID, const BlockID blockID, const string &name, const string &desc, const xd::Sprite *icon) :
		ItemData(itemID, name, desc, icon, 255, false),
		m_blockID(blockID)
	{
	}

	void use(Player *player)
	{
	}

private:

	const BlockID m_blockID;
};

void ItemData::init()
{
	s_itemData[ITEM_OAK_BLOCK] = new BlockItem(ITEM_OAK_BLOCK, BLOCK_WOOD, "Oak block", "A block of oak", new xd::Sprite(xd::ResourceManager::get<xd::Texture2D>(":/Sprites/ItemIcons/OakBlock.png")));
}