#include "Hotbar.h"
#include "Constants.h"
#include "Game/Game.h"
#include "Game/ItemContainer.h"
#include "Items/ItemData.h"
#include "Entities/Player.h"

Hotbar::Hotbar(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(scene, gameOverlay),
	m_gameOverlay(gameOverlay),
	m_slotSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlot.png")),
	m_slotSelectedSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlotSelected.png")),
	m_backgroundSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/Hotbar.png")),
	m_font(ResourceManager::get<Font>(UI_INVENTORY_FONT)),
	m_selectedSlot(0)
{
	m_slotSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_slotSprite.setDepth(-1.f);
	m_slotSelectedSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_slotSelectedSprite.setDepth(-1.f);
	m_backgroundSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_backgroundSprite.setDepth(-2.f);

	m_font->setDepth(1.f);

	setAnchor(Vector2(0.5f, 0.98f));
	setSize(Vector2(490.f, 56.f)/m_parent->getSize());
	setPosition(Vector2(.0f));

	// Bind keys to item slots
	Input::bind(XD_KEY_1, bind(&Hotbar::setSelectedSlot, this, 0));
	Input::bind(XD_KEY_2, bind(&Hotbar::setSelectedSlot, this, 1));
	Input::bind(XD_KEY_3, bind(&Hotbar::setSelectedSlot, this, 2));
	Input::bind(XD_KEY_4, bind(&Hotbar::setSelectedSlot, this, 3));
	Input::bind(XD_KEY_5, bind(&Hotbar::setSelectedSlot, this, 4));
	Input::bind(XD_KEY_6, bind(&Hotbar::setSelectedSlot, this, 5));
	Input::bind(XD_KEY_7, bind(&Hotbar::setSelectedSlot, this, 6));
	Input::bind(XD_KEY_8, bind(&Hotbar::setSelectedSlot, this, 7));
	Input::bind(XD_KEY_9, bind(&Hotbar::setSelectedSlot, this, 8));
	Input::bind(XD_KEY_0, bind(&Hotbar::setSelectedSlot, this, 9));
}

void Hotbar::update(const float delta)
{
	setSize(Vector2(490.f, 56.f)/m_parent->getSize());

	UiObject::update(delta);
}

void Hotbar::draw(SpriteBatch *spriteBatch, const float alpha)
{
	if(!m_gameOverlay->getPlayer()) return;

	// Get size and position
	Vector2 position = getPosition();
	Vector2 size = getSize();
	
	m_backgroundSprite.setPosition(position);
	m_backgroundSprite.setSize(size);
	spriteBatch->drawSprite(m_backgroundSprite);

	ItemContainer *itemContainer = m_gameOverlay->getPlayer()->getItemContainer();
	for(uint i = 0; i < 10; ++i)
	{
		Sprite &sprite = i == m_selectedSlot ? m_slotSelectedSprite : m_slotSprite;
		sprite.setPosition(position + Vector2(8.f + i * 48.f, 7.f));
		spriteBatch->drawSprite(sprite);

		ItemContainer::Slot &slot = itemContainer->getSlotAt(i);
		if(slot.item != ITEM_NONE)
		{
			spriteBatch->drawSprite(Sprite(ItemData::get(slot.item)->getIconTexture(), Rect(position.x + 13.f + i * 48.f, position.y + 12.f, 32.f, 32.f)));
			if(slot.amount > 1)
			{
				spriteBatch->drawText(Vector2(position.x + 11.f + i * 48.f, position.y + 10.f), util::intToStr(slot.amount), m_font);
			}
		}
	}
}

void Hotbar::keyPressEvent(const VirtualKey key)
{
	if(key == XD_LMB)
	{
		Vector2 position = getPosition();
		for(uint i = 0; i < 10; ++i)
		{
			Rect rect(position.x + 8.f + i * 48.f, position.y + 7.f, 42.0f, 42.0f);
			if(rect.contains(Input::getPosition()))
			{
				m_selectedSlot = i;
				break;
			}
		}
	}
	else if(key == XD_RMB)
	{
		Vector2 position = getPosition();
		for(uint i = 0; i < 10; ++i)
		{
			Rect rect(position.x + 8.f + i * 48.f, position.y + 7.f, 42.0f, 42.0f);
			if(rect.contains(Input::getPosition()))
			{
				m_gameOverlay->takeItem(m_gameOverlay->getPlayer()->getItemContainer(), i);
				break;
			}
		}
	}
}

void Hotbar::mouseWheelEvent(const int delta)
{
	if(delta < 0)
	{
		if(m_selectedSlot == 9)
		{
			m_selectedSlot = 0;
		}
		else
		{
			m_selectedSlot += 1;
		}
	}
	else
	{
		if(m_selectedSlot == 0)
		{
			m_selectedSlot = 9;
		}
		else
		{
			m_selectedSlot -= 1;
		}
	}
}