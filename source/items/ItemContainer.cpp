#include "ItemContainer.h"
#include "Game.h"

ItemContainer::ItemContainer(const int size) :
	m_size(size)
{
	m_items = new ItemSlot[size];
}

bool ItemContainer::addItem(ItemData *item, const uint amount)
{
	if(item == nullptr || amount == 0)
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
	uint slot = 0;
	for(; slot < m_size; slot++)
	{
		if(m_items[slot].isEmpty())
		{
			return slot;
		}
	}
	return -1;
}