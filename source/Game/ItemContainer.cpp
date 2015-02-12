#include "ItemContainer.h"
#include "Game.h"

ItemContainer::ItemContainer(const int size) :
	m_size(size)
{
	m_items = new ItemSlot[size];
}

bool ItemContainer::addItem(ItemID item, const uint amount)
{
	if(amount == 0)
	{
		return false;
	}

	int slot = findEmptySlot();
	if(slot < 0)
	{
		return false;
	}

	m_items[slot].set(item, amount);

	return true;
}

int ItemContainer::findEmptySlot() const
{
	for(int slot = 0; slot < m_size; slot++)
	{
		if(m_items[slot].isEmpty())
		{
			return slot;
		}
	}
	return -1;
}