#include "Hotbar.h"
#include "Constants.h"
#include "Game/Game.h"
#include "GameOverlay.h"
#include "Game/Storage.h"
#include "Items/ItemData.h"
#include "Entities/Dynamic/Player.h"

Hotbar::Hotbar(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(scene, gameOverlay),
	m_gameOverlay(gameOverlay),
	m_backgroundSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/Hotbar.png")),
	m_slotSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlot.png")),
	m_slotSelectedSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlotSelected.png")),
	m_font(ResourceManager::get<Font>(UI_INVENTORY_FONT))
{
	m_font->setColor(Color(0, 0, 0, 255));
	m_font->setDepth(1.f);
	m_slotSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_slotSprite.setDepth(-1.f);
	m_slotSelectedSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_slotSelectedSprite.setDepth(-1.f);
	m_backgroundSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_backgroundSprite.setDepth(-2.f);

	setAnchor(Vector2(0.5f, 0.98f));
	setSize(Vector2(490.f, 56.f)/m_parent->getSize());
	setPosition(Vector2(.0f));
}

Hotbar::~Hotbar()
{
}

void Hotbar::update(const float delta)
{
	setSize(Vector2(490.f, 56.f)/m_parent->getSize());

	UiObject::update(delta);
}

void Hotbar::draw(SpriteBatch *spriteBatch, const float alpha)
{
	Player *player = m_gameOverlay->getPlayer();
	if(!player || m_gameOverlay->m_hidden) return;

	// Get size and position
	Vector2 position = getPosition();
	Vector2 size = getSize();
	
	m_backgroundSprite.setPosition(position);
	m_backgroundSprite.setSize(size);
	spriteBatch->drawSprite(m_backgroundSprite);
	
	for(int i = 0; i < 10; ++i)
	{
		Sprite &sprite = i == player->getSelectedSlot() ? m_slotSelectedSprite : m_slotSprite;
		sprite.setPosition(position + Vector2(8.f + i * 48.f, 7.f));
		spriteBatch->drawSprite(sprite);
		m_gameOverlay->getPlayer()->getStorage()->getSlotAt(i)->drawItem(position + Vector2(8.f + i * 48.f, 7.f), spriteBatch, m_font);
	}
}

void Hotbar::setSelectedSlot(int action, const uint slot)
{
	if(action != GLFW_PRESS) return;
	m_gameOverlay->getPlayer()->setSelectedSlot(slot);
}

void Hotbar::mouseWheelEvent(const int delta)
{
	Player *player = m_gameOverlay->getPlayer();
	player->setSelectedSlot(math::mod(player->getSelectedSlot() + (delta < 0 ? 1 : -1), 10));
}

void Hotbar::keyEvent(const KeyEvent & event)
{
	if(!event.isAction(KeyEvent::PRESS)) return;

	const VirtualKey key = event.getKey();
	if(key == XD_MOUSE_BUTTON_LEFT || key == XD_MOUSE_BUTTON_RIGHT)
	{
		Vector2 position = getPosition();
		for(uint i = 0; i < 10; ++i)
		{
			Rect rect(position.x + 8.f + i * 48.f, position.y + 7.f, 42.f, 42.f);
			if(rect.contains(Input::getPosition()))
			{
				if(key == XD_MOUSE_BUTTON_LEFT)
				{
					if(m_gameOverlay->getPlayer()->getHeldItem()->isEmpty())
					{
						// Left click on a slot with no held item -> Select the slot
						m_gameOverlay->getPlayer()->setSelectedSlot(i);
						break;
					}
				}
				m_gameOverlay->performSlotAction(m_gameOverlay->getPlayer()->getStorage()->getSlotAt(i), key);

				break;
			}
		}
	}
}