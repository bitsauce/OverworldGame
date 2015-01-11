#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <x2d/x2d.h>

class Player;
enum ItemID;

class ItemData
{
	friend class GameManager;
public:
	ItemData(const ItemID id, const string &name, const string &desc, const xd::Sprite *icon, const uint maxStack, const bool singleShot = false) :
		m_id(id),
		m_name(name),
		m_desc(desc),
		m_icon(icon),
		m_maxStack(maxStack),
		m_singleShot(singleShot)
	{
	}

	virtual void use(Player *player) = 0;

private:

	static void init();

	const ItemID m_id;
	const string m_name;
	const string m_desc;
	const uint m_maxStack;
	const xd::Sprite *m_icon;
	const bool m_singleShot;

	static vector<ItemData*> s_itemData;
};

#endif // ITEM_DATA_H