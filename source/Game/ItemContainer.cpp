#include "ItemContainer.h"
#include "Game.h"

ItemContainer::ItemContainer(const uint size) :
	m_size(size)
{
	m_items = new ItemSlot[size];
}

bool ItemContainer::addItem(const ItemID item, int amount)
{
	if(amount > 0)
	{
		// Find a stack of the same item to add this to
		for(uint i = 0; i < m_size; ++i)
		{
			if(m_items[i].item == item)
			{
				m_items[i].amount += amount;
				amount = m_items[i].amount - ItemData::get(item)->getMaxStack();
				if(amount <= 0) {
					return true;
				}
				m_items[i].amount -= amount;
			}
			else if(m_items[i].item == ITEM_NONE)
			{
				m_items[i].set(item, amount);
				return true;
			}
		}
	}
	return false;
}


bool ItemContainer::removeItem(const ItemID item, const uint amount)
{
	for(uint i = 0; i < m_size; ++i)
	{
		if(m_items[i].item == item)
		{
			m_items[i].amount -= amount;
			if(m_items[i].amount <= 0)
			{
				m_items[i].set(ITEM_NONE, 0);
			}
			return true;
		}
	}
	return false;
}

int ItemContainer::findEmptySlot() const
{
	for(uint slot = 0; slot < m_size; ++slot)
	{
		if(m_items[slot].isEmpty())
		{
			return slot;
		}
	}
	return -1;
}