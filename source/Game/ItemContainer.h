#ifndef ITEM_CONTAINER_H
#define ITEM_CONTAINER_H

#include "Config.h"
#include "Constants.h"

class ItemData;

class ItemContainer
{
public:
	ItemContainer(const uint size);

	bool addItem(const ItemID item, const uint amount = 1);
	bool removeItem(const ItemID item, const uint amount = 1);

	ItemID getItemAt(const int idx) const { return m_items[idx].item; }
	uint getItemAmountAt(const int idx) const { return m_items[idx].amount; }

	uint getSize() const { return m_size; }

private:
	int findEmptySlot() const;

	struct ItemSlot
	{
		ItemSlot() :
			item(ITEM_NONE),
			amount(0)
		{
		}

		void set(ItemID item, const uint amount) { this->item = item; this->amount = amount; }
		bool isEmpty() const { return item == ITEM_NONE; }

		ItemID item;
		uint amount;
	};

	ItemSlot *m_items;
	const uint m_size;
};

#endif // ITEM_CONTAINER_H