#include "Inventory.h"
#include "Game/Game.h"
#include "Items/ItemData.h"

Inventory::Inventory(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(scene, gameOverlay),
	m_gameOverlay(gameOverlay),
	m_itemSlotSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlot.png")),
	m_backgroundSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/Inventory.png")),
	m_font(ResourceManager::get<Font>(UI_INVENTORY_FONT)),
	m_itemContainer(100),
	m_fadeInAlpha(0.0f),
	m_animationTime(0.0f),
	m_animationDuration(0.5f),
	m_interpolateStartY(0.0f),
	m_interpolateEndY(0.0f),
	m_interpolateStartAlpha(0.0f),
	m_interpolateEndAlpha(0.0f),
	m_showing(false)
{
	m_itemSlotSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_itemSlotSprite.setDepth(-1.f);
	m_backgroundSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_backgroundSprite.setDepth(-3.f);
	
	Input::bind(XD_KEY_I, bind(&Inventory::toggle, this));

	setAnchor(Vector2(0.5f, 0.98f));
	setSize(Vector2(976.0f, 310.0f)/m_parent->getSize());
	setPosition(Vector2(0.0f));
}

void Inventory::toggle()
{
	m_showing = !m_showing;
}

void Inventory::show()
{
	m_showing = true;
}

void Inventory::hide()
{
	m_showing = false;
}

void Inventory::update(const float delta)
{
	setSize(Vector2(976.0f, 310.0f)/m_parent->getSize());

	m_animationTime += m_showing ? delta : -delta;
	if(m_animationTime > m_animationDuration)
	{
		m_animationTime = m_animationDuration;
	}
	else if(m_animationTime < 0.0f)
	{
		m_animationTime = 0.0f;
	}

	m_interpolateStartY = m_rect.position.y;
	m_interpolateEndY = math::lerp(m_rect.size.y + 0.02f, 0.0f, m_animationTime/m_animationDuration);
	m_interpolateStartAlpha = m_fadeInAlpha;
	m_interpolateEndAlpha = math::lerp(0.0f, 1.0f, m_animationTime/m_animationDuration);
	
	UiObject::update(delta);
}

void Inventory::draw(SpriteBatch *spriteBatch, const float alpha)
{
	m_rect.position.y = math::lerp(m_interpolateStartY, m_interpolateEndY, alpha);
	m_fadeInAlpha = math::lerp(m_interpolateStartAlpha, m_interpolateEndAlpha, alpha);

	if(m_animationTime == 0.0f) return;

	Color fadeColor(255, 255, 255, 255 * m_fadeInAlpha);
	m_backgroundSprite.setColor(fadeColor);
	m_itemSlotSprite.setColor(fadeColor);

	Vector2 position = getPosition();
	Vector2 size = getSize();

	m_backgroundSprite.setPosition(position);
	spriteBatch->drawSprite(m_backgroundSprite);

	for(int y = 0; y < 5; ++y)
	{
		for(int x = 0; x < 20; ++x)
		{
			m_itemSlotSprite.setPosition(position + Vector2(8.f + x * 48.f, 7.f + y * 48.f));
			spriteBatch->drawSprite(m_itemSlotSprite);

			ItemContainer::Slot &slot = m_itemContainer.getSlotAt(x + y * 20);
			if(slot.item != ITEM_NONE)
			{
				spriteBatch->drawSprite(Sprite(ItemData::get(slot.item)->getIconTexture(), Rect(position.x + 13.f + x * 48.f, position.y + 12.f + y * 48.f, 32.f, 32.f)));
				if(slot.amount > 1)
				{
					spriteBatch->drawText(Vector2(position.x + 11.f + x * 48.f, position.y + 10.f + y * 48.0f), util::intToStr(slot.amount), m_font);
				}
			}
		}
	}
}

void Inventory::keyPressEvent(const VirtualKey key)
{
	/*if(key == XD_LMB)
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
	else */if(key == XD_RMB)
	{
		Vector2 position = getPosition();
		for(int y = 0; y < 5; ++y)
		{
			for(int x = 0; x < 20; ++x)
			{
				Rect rect(position.x + 8.f + x * 48.f, position.y + 7.f + y * 48.f, 42.0f, 42.0f);
				if(rect.contains(Input::getPosition()))
				{
					m_gameOverlay->takeItem(&m_itemContainer, x + y * 20);
					break;
				}
			}
		}
	}
}