#ifndef ITEM_CONTAINER_H
#define ITEM_CONTAINER_H

#include "Config.h"

class ItemData;
enum ItemID;

class ItemContainer
{
public:
	ItemContainer(const uint size);

	struct Slot
	{
		Slot();

		void set(ItemID item, const uint amount);
		bool isEmpty() const;

		ItemID item;
		uint amount;
	};

	int addItem(const ItemID item, int amount = 1);
	bool removeItem(const ItemID item, const uint amount = 1);

	Slot &getSlotAt(const int idx) const { return m_items[idx]; }
	ItemID getItemAt(const int idx) const { return m_items[idx].item; }
	uint getItemAmountAt(const int idx) const { return m_items[idx].amount; }
	void setSlotAt(const int idx, Slot slot) { m_items[idx] = slot; }

	void removeItemsAt(const int idx);

	uint getSize() const { return m_size; }

private:
	int findEmptySlot() const;

	Slot *m_items;
	const uint m_size;
};

#endif // ITEM_CONTAINER_H