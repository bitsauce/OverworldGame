#include "Hotbar.h"
#include "Constants.h"
#include "Game/Game.h"
#include "GameOverlay.h"
#include "Game/Storage.h"
#include "Items/ItemData.h"
#include "Entities/Player.h"

Hotbar::Hotbar(GameOverlay *gameOverlay) :
	UiObject(gameOverlay),
	m_gameOverlay(gameOverlay),
	m_backgroundSprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Inventory/Hotbar")),
	m_slotSprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Inventory/ItemSlot")),
	m_slotSelectedSprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Inventory/ItemSlotSelected")),
	m_font(Game::GetInstance()->getResourceManager()->get<Font>("Fonts/Inventory"))
{
	m_font->setColor(Color(0, 0, 0, 255));
	m_font->setDepth(1.f);
	m_slotSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_slotSprite.setDepth(-1.f);
	m_slotSelectedSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_slotSelectedSprite.setDepth(-1.f);
	m_backgroundSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_backgroundSprite.setDepth(-2.f);

	setAnchor(Vector2F(0.5f, 0.98f));
	setSize(Vector2F(490.f, 56.f) / gameOverlay->getSize());
	setPosition(Vector2F(.0f));
}

Hotbar::~Hotbar()
{
}

void Hotbar::onTick(TickEvent *e)
{
	setSize(Vector2F(490.f, 56.f) / m_gameOverlay->getSize());

	UiObject::onTick(e);
}

void Hotbar::onDraw(DrawEvent *e)
{
	Player *player = m_gameOverlay->getPlayer();
	if(!player || m_gameOverlay->m_hidden) return;

	// Get size and position
	Vector2F position = getDrawPosition();
	Vector2F size = getDrawSize();

	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	
	m_backgroundSprite.setPosition(position);
	m_backgroundSprite.setSize(size);
	spriteBatch->drawSprite(m_backgroundSprite);
	
	for(int i = 0; i < 10; ++i)
	{
		Sprite &sprite = i == player->getSelectedSlot() ? m_slotSelectedSprite : m_slotSprite;
		sprite.setPosition(position + Vector2F(8.f + i * 48.f, 7.f));
		spriteBatch->drawSprite(sprite);
		m_gameOverlay->getPlayer()->getStorage()->getSlotAt(i)->drawItem(position + Vector2F(8.f + i * 48.f, 7.f), spriteBatch, m_font);
	}
}

void Hotbar::setSelectedSlot(int action, const uint slot)
{
	//if(action != GLFW_PRESS) return;
	m_gameOverlay->getPlayer()->setSelectedSlot(slot);
}

void Hotbar::onMouseWheelEvent(MouseEvent *e)
{
	Player *player = m_gameOverlay->getPlayer();
	player->setSelectedSlot(math::mod(player->getSelectedSlot() + (e->getWheelY() < 0 ? 1 : -1), 10));
}

void Hotbar::onKeyEvent(KeyEvent *e)
{
	if(e->getType() != KeyEvent::DOWN) return;

	const Keycode key = e->getKeycode();
	/*if(key == XD_MOUSE_BUTTON_LEFT || key == XD_MOUSE_BUTTON_RIGHT)
	{
		Vector2F position = getPosition();
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
	}*/
}