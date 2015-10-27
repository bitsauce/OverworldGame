#include "Inventory.h"
#include "Game/Game.h"
#include "GameOverlay.h"
#include "Items/ItemData.h"
#include "Game/Storage.h"
#include "Entities/Dynamic/Player.h"

Inventory::Inventory(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(scene, gameOverlay),
	m_gameOverlay(gameOverlay),
	m_itemSlotSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlot.png")),
	m_backgroundSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/Inventory.png")),
	m_font(ResourceManager::get<Font>(UI_INVENTORY_FONT)),
	m_fadeInAlpha(0.0f),
	m_animationTime(0.0f),
	m_animationDuration(0.5f),
	m_interpolateStartY(0.0f),
	m_interpolateEndY(0.0f),
	m_interpolateStartAlpha(0.0f),
	m_interpolateEndAlpha(0.0f),
	m_showing(false)
{
	m_font->setDepth(1.f);
	m_itemSlotSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_itemSlotSprite.setDepth(-3.f);
	m_backgroundSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_backgroundSprite.setDepth(-4.f);

	setAnchor(Vector2(0.5f, 0.85f));
	setSize(Vector2(970.0f, 248.0f)/m_parent->getSize());
	setPosition(Vector2(0.0f));
}

void Inventory::toggle(int action)
{
	if(action != GLFW_PRESS) return;
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
	setSize(Vector2(970.0f, 248.0f)/m_parent->getSize());

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
	m_interpolateEndY = math::lerp(m_rect.size.y + 0.15f, 0.0f, m_animationTime/m_animationDuration);
	m_interpolateStartAlpha = m_fadeInAlpha;
	m_interpolateEndAlpha = math::lerp(0.0f, 1.0f, m_animationTime/m_animationDuration);
	
	UiObject::update(delta);
}

void Inventory::draw(SpriteBatch *spriteBatch, const float alpha)
{
	m_rect.position.y = math::lerp(m_interpolateStartY, m_interpolateEndY, alpha);
	m_fadeInAlpha = math::lerp(m_interpolateStartAlpha, m_interpolateEndAlpha, alpha);

	if(m_animationTime == 0.0f) return;

	Color fadeColor(255, 255, 255, (uchar) (255 * m_fadeInAlpha));
	m_backgroundSprite.setColor(fadeColor);
	m_itemSlotSprite.setColor(fadeColor);

	Vector2 position = getPosition();
	Vector2 size = getSize();
	
	m_backgroundSprite.setSize(size); // TODO: Remove once the sprite is of correct size

	m_backgroundSprite.setPosition(position);
	spriteBatch->drawSprite(m_backgroundSprite);

	Bag *bag = m_gameOverlay->getPlayer()->getBag();
	for(uint y = 0; y < bag->getHeight(); ++y)
	{
		for(uint x = 0; x < bag->getWidth(); ++x)
		{
			m_itemSlotSprite.setPosition(position + Vector2(8.f + x * 48.f, 7.f + y * 48.f));
			spriteBatch->drawSprite(m_itemSlotSprite);

			bag->getStorage()->getSlotAt(x + y * bag->getWidth())->drawItem(position + Vector2(8.f + x * 48.f, 7.f + y * 48.f), spriteBatch, m_font);
		}
	}
}

void Inventory::keyEvent(const KeyEvent & event)
{
	if(!event.isAction(KeyEvent::PRESS)) return;

	const VirtualKey key = event.getKey();
	if (!m_gameOverlay->getPlayer()) return;
	if(key == XD_MOUSE_BUTTON_LEFT || key == XD_MOUSE_BUTTON_RIGHT)
	{
		Vector2 position = getPosition();
		Bag *bag = m_gameOverlay->getPlayer()->getBag();
		for(uint y = 0; y < bag->getHeight(); ++y)
		{
			for(uint x = 0; x < bag->getWidth(); ++x)
			{
				Rect rect(position.x + 8.0f + x * 48.0f, position.y + 7.f + y * 48.0f, 42.0f, 42.0f);
				if(rect.contains(Input::getPosition()))
				{
					m_gameOverlay->performSlotAction(m_gameOverlay->getPlayer()->getBag()->getStorage()->getSlotAt(x + y * bag->getWidth()), key);
					break;
				}
			}
		}
	}
}