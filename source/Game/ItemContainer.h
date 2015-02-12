#ifndef ITEM_CONTAINER_H
#define ITEM_CONTAINER_H

#include "Config.h"
#include "Constants.h"

class ItemData;

class ItemContainer
{
public:
	ItemContainer(const int size);

	bool addItem(ItemID item, const uint amount = 1);

	ItemID getItemAt(const int idx) const { return m_items[idx].item; }
	uint getItemAmountAt(const int idx) const { return m_items[idx].amount; }

	int getSize() const { return m_size; }

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
	const int m_size;
};

#endif // ITEM_CONTAINER_H