#include "ItemContainer.h"
#include "Items/ItemData.h"
#include "Constants.h"

ItemContainer::ItemContainer(const uint size) :
	m_size(size)
{
	m_items = new ItemSlot[size];
}

int ItemContainer::addItem(const ItemID item, int amount)
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
					return 0;
				}
				m_items[i].amount -= amount;
			}
			else if(m_items[i].item == ITEM_NONE)
			{
				m_items[i].set(item, amount);
				return 0;
			}
		}
	}
	return amount;
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

void ItemContainer::removeItemsAt(const int idx)
{
	m_items[idx].set(ITEM_NONE, 0);
}

ItemContainer::ItemSlot::ItemSlot() :
	item(ITEM_NONE),
	amount(0)
{
}

void ItemContainer::ItemSlot::set(ItemID item, const uint amount)
{
	this->item = item;
	this->amount = amount;
}

bool ItemContainer::ItemSlot::isEmpty() const
{
	return item == ITEM_NONE;
}