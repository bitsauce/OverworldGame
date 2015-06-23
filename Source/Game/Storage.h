#ifndef ITEM_CONTAINER_H
#define ITEM_CONTAINER_H

#include "Config.h"
//>REMOVE ME<
class ItemData;
enum ItemID;

class Storage
{
public:
	class Slot
	{
	public:
		Slot();

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

	Storage(const uint size);

	int addItem(const ItemID item, int amount = 1);
	int removeItem(const ItemID item, int amount = 1);

	Storage::Slot *getSlotAt(const int idx) { return &m_slots[idx]; }
	uint getSize() const { return m_size; }
	void setNext(Storage *next) { m_next = next; }
	Storage *getNext() const { return m_next; }

private:
	int findEmptySlot() const;

	Storage::Slot *m_slots;
	const uint m_size;
	Storage *m_next;
};

#endif // ITEM_CONTAINER_H