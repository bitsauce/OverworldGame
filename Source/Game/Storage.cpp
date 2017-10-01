#include "Storage.h"
#include "Items/ItemData.h"
#include "Constants.h"

Storage::Storage(const uint size) :
	m_size(size),
	m_next(nullptr)
{
	m_slots = new Slot[size];
}

int Storage::addItem(const ItemID item, int amount)
{
	if(amount > 0)
	{
		// Find a stack of the same item to add this to
		for(uint i = 0; i < m_size; ++i)
		{
			if(m_slots[i].getItem() == item)
			{
				amount = m_slots[i].inc(amount);
				if(amount <= 0)
				{
					return 0;
				}
			}
			else if(m_slots[i].getItem() == ITEM_NONE)
			{
				m_slots[i].set(item, amount);
				return 0;
			}
		}
	}
	return m_next != nullptr ? m_next->addItem(item, amount) : amount;
}

int Storage::removeItem(const ItemID item, int amount)
{
	for(uint i = 0; i < m_size; ++i)
	{
		if(m_slots[i].getItem() == item)
		{
			amount = m_slots[i].dec(amount);
			if(amount <= 0)
			{
				return 0;
			}
		}
	}
	return m_next != nullptr ? m_next->removeItem(item, amount) : amount;
}

int Storage::findEmptySlot() const
{
	for(uint slot = 0; slot < m_size; ++slot)
	{
		if(m_slots[slot].isEmpty())
		{
			return slot;
		}
	}
	return -1;
}

Storage::Slot::Slot() :
	m_item(ITEM_NONE),
	m_amount(0)
{
}

void Storage::Slot::set(ItemID item, const uint amount)
{
	m_item = item;
	m_amount = amount;
}

bool Storage::Slot::isEmpty() const
{
	return m_item == ITEM_NONE || m_amount <= 0;
}

void Storage::Slot::drawItem(const Vector2F position, SpriteBatch *spriteBatch, Resource<Font> font)
{
	if(!isEmpty())
	{
		spriteBatch->drawSprite(Sprite(ItemData::Get(m_item)->getIconTexture(), RectF(position.x, position.y, 42.f, 42.f)));
		if(m_amount > 1)
		{
			spriteBatch->drawText(Vector2F(position.x + 3.f, position.y + 3.f), util::intToStr(m_amount), font.get());
		}
	}
}

int Storage::Slot::inc(const uint amount)
{
	const int maxStack = ItemData::Get(m_item)->getMaxStack();

	m_amount += amount;
	int rest = m_amount - maxStack;
	if(m_amount > maxStack)
	{
		m_amount = maxStack;
	}

	return max(rest, 0);
}

int Storage::Slot::dec(const uint amount)
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