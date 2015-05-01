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
			if(m_items[i].getItem() == item)
			{
				amount = m_items[i].inc(amount);
				if(amount <= 0) {
					return 0;
				}
			}
			else if(m_items[i].getItem() == ITEM_NONE)
			{
				m_items[i].set(item, amount);
				return 0;
			}
		}
	}
	return amount;
}

int ItemContainer::removeItem(const ItemID item, int amount)
{
	for(uint i = 0; i < m_size; ++i)
	{
		if(m_items[i].getItem() == item)
		{
			amount = m_items[i].dec(amount);
			if(amount <= 0) {
				return 0;
			}
		}
	}
	return amount;
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

ItemSlot::ItemSlot() :
	m_item(ITEM_NONE),
	m_amount(0)
{
}

void ItemSlot::set(ItemID item, const uint amount)
{
	m_item = item;
	m_amount = amount;
}

bool ItemSlot::isEmpty() const
{
	return m_item == ITEM_NONE || m_amount <= 0;
}

void ItemSlot::drawItem(const Vector2 position, SpriteBatch *spriteBatch, FontPtr font)
{
	if(!isEmpty())
	{
		spriteBatch->drawSprite(Sprite(ItemData::get(m_item)->getIconTexture(), Rect(position.x + 13.f, position.y + 12.f, 32.f, 32.f)));
		if(m_amount > 1)
		{
			spriteBatch->drawText(Vector2(position.x + 11.f, position.y + 10.f), util::intToStr(m_amount), font);
		}
	}
}

int ItemSlot::inc(const uint amount)
{
	const int maxStack = ItemData::get(m_item)->getMaxStack();

	m_amount += amount;
	int rest = m_amount - maxStack;
	if(m_amount > maxStack)
	{
		m_amount = maxStack;
	}

	return rest;
}

int ItemSlot::dec(const uint amount)
{
	m_amount -= amount;

	int rest = 0;
	if(m_amount <= 0)
	{
		rest = -m_amount;
		m_amount = 0;
		m_item = ITEM_NONE;
	}

	return rest;
}