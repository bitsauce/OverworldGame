#ifndef ITEM_CONTAINER_H
#define ITEM_CONTAINER_H

#include "Config.h"

class ItemData;
enum ItemID;

class ItemSlot
{
public:
	ItemSlot();

	void set(const ItemID item, const uint amount);
	
	int inc(const uint amount = 1);
	int dec(const uint amount = 1);

	ItemID getItem() const { return m_item; }
	int getAmount() const { return m_amount; }
	bool isEmpty() const;

	void drawItem(const Vector2 position, SpriteBatch *spriteBatch, FontPtr font);

private:
	ItemID m_item;
	int m_amount;
};

class ItemStorage
{
public:
	ItemStorage(const uint size);

	int addItem(const ItemID item, int amount = 1);
	int removeItem(const ItemID item, int amount = 1);

	ItemSlot &getSlotAt(const int idx) { return m_items[idx]; }
	uint getSize() const { return m_size; }
	void setNext(ItemStorage *next) { m_next = next; }
	ItemStorage *getNext() const { return m_next; }

private:
	int findEmptySlot() const;

	ItemSlot *m_items;
	const uint m_size;
	ItemStorage *m_next;
};

#endif // ITEM_CONTAINER_H