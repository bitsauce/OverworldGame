#ifndef ITEM_CONTAINER_H
#define ITEM_CONTAINER_H

#include <x2d/x2d.h>

class ItemData;

class ItemContainer
{
public:
	ItemContainer(const int size);

	bool addItem(ItemData *item, const uint amount = 1);
	int getSize() const { return m_size; }

private:
	int findEmptySlot() const;

	struct ItemSlot
	{
		ItemSlot() :
			item(nullptr),
			amount(0)
		{
		}

		void set(ItemData *item, const uint amount) { this->item = item; this->amount = amount; }
		bool isEmpty() const { return item == nullptr; }

		ItemData *item;
		uint amount;
	};

	ItemSlot *m_items;
	const int m_size;
};

#endif // ITEM_CONTAINER_H