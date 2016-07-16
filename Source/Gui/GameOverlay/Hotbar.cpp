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
	m_itemAmountFont(Game::GetInstance()->getResourceManager()->get<Font>("Fonts/Inventory")),
	m_itemDescFont(Game::GetInstance()->getResourceManager()->get<Font>("Fonts/Debug")),
	m_mousePosition(0.0f)
{
	m_itemAmountFont->setColor(Color(0, 0, 0, 255));
	m_itemAmountFont->setDepth(1.f);
	m_itemDescFont->setColor(Color(255, 255, 255, 255));
	m_itemDescFont->setDepth(1.f);
	m_slotSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_slotSprite.setDepth(-1.f);
	m_slotSelectedSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_slotSelectedSprite.setDepth(-1.f);
	m_backgroundSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_backgroundSprite.setDepth(-2.f);

	setAnchor(0.5f, 0.95f);
	setSize(Vector2F(490.f, 56.f) / gameOverlay->getDrawSize());
	setPosition(0.0f, 0.0f);
	setOrigin(0.5f, 0.5f);
}

Hotbar::~Hotbar()
{
}

void Hotbar::onTick(TickEvent *e)
{
	if(m_mouseWheel != 0)
	{
		Player *player = m_gameOverlay->getPlayer();
		player->setSelectedSlot(math::mod(player->getSelectedSlot() + (m_mouseWheel < 0 ? 1 : -1), 10));
		m_mouseWheel = 0;
	}

	setSize(Vector2F(490.f, 56.f) / m_gameOverlay->getDrawSize());
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
	
	// Draw hotbar item slots
	for(int i = 0; i < 10; ++i)
	{
		// Get and draw slot sprite
		Sprite &sprite = i == player->getSelectedSlot() ? m_slotSelectedSprite : m_slotSprite;
		sprite.setPosition(position + Vector2F(8.f + i * 48.f, 7.f));
		spriteBatch->drawSprite(sprite);

		// Get item slot
		Storage::Slot *slot = m_gameOverlay->getPlayer()->getStorage()->getSlotAt(i);

		// Draw item in slot
		slot->drawItem(position + Vector2F(8.f + i * 48.f, 7.f), spriteBatch, m_itemAmountFont);

		// Check if mouse is hovering over this item
		if(!slot->isEmpty() && RectF(sprite.getPosition(), sprite.getSize()).contains(m_mousePosition))
		{
			ItemData *itemData = ItemData::get(slot->getItem());
			m_itemDescFont->drawBox(spriteBatch, sprite.getPosition() - Vector2F(0.0f, m_itemDescFont->getHeight() * 2), 200, itemData->getDesc());
			m_itemDescFont->draw(spriteBatch, sprite.getPosition() - Vector2F(0.0f, m_itemDescFont->getHeight()*3), itemData->getName());
		}
	}
}

void Hotbar::setSelectedSlot(KeyEvent *e, const uint slot)
{
	if(e->getType() != KeyEvent::DOWN) return;
	m_gameOverlay->getPlayer()->setSelectedSlot(slot);
}

void Hotbar::onMouseWheel(MouseEvent *e)
{
	m_mouseWheel = e->getWheelY();
}

void Hotbar::onMouseMove(MouseEvent *e)
{
	m_mousePosition = e->getPosition();
}

void Hotbar::onMouseDown(MouseEvent *e)
{
	const MouseButton button = e->getButton();
	if(button == SAUCE_MOUSE_BUTTON_LEFT || button == SAUCE_MOUSE_BUTTON_RIGHT)
	{
		Vector2F position = getDrawPosition();
		for(uint i = 0; i < 10; ++i)
		{
			RectF rect(position.x + 8.f + i * 48.f, position.y + 7.f, 42.f, 42.f);
			if(rect.contains(m_mousePosition))
			{
				if(button == SAUCE_MOUSE_BUTTON_LEFT)
				{
					if(m_gameOverlay->getPlayer()->getHeldItem()->isEmpty())
					{
						// Left click on a slot with no held item -> Select the slot
						m_gameOverlay->getPlayer()->setSelectedSlot(i);
						break;
					}
				}
				//m_gameOverlay->performSlotAction(m_gameOverlay->getPlayer()->getStorage()->getSlotAt(i), button);
				break;
			}
		}
	}
}