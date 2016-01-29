#include "Crafting.h"
#include "Gui/GameOverlay/GameOverlay.h"
#include "Game/Game.h"
#include "Items/ItemData.h"

Crafting::Crafting(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(gameOverlay),
	m_gameOverlay(gameOverlay),
	m_font(Game::GetInstance()->getResourceManager()->get<Font>("Fonts/Inventory")),
	m_itemSlotSprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Inventory/ItemSlot.png").get()),
	m_backgroundSprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Inventory/Inventory.png").get()),
	m_storage(9)
{
	m_font->setDepth(1.f);
	m_itemSlotSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_itemSlotSprite.setDepth(-1.f);
	m_backgroundSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_backgroundSprite.setDepth(-3.f);

	setAnchor(Vector2F(0.75f, 0.25f));
	setSize(Vector2F(154.0f, 152.0f) / gameOverlay->getSize());
	setPosition(Vector2F(0.0f));
}

void Crafting::onTick(TickEvent *e)
{
	if(!m_gameOverlay->isCrafting()) return;

	setSize(Vector2F(154.0f, 152.0f) / m_gameOverlay->getSize());

	UiObject::onTick(e);
}

void Crafting::onDraw(DrawEvent *e)
{
	if(!m_gameOverlay->isCrafting()) return;

	Vector2F position = getPosition();
	Vector2F size = getSize();

	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	GraphicsContext *context = e->getGraphicsContext();

	m_backgroundSprite.setPosition(position);
	m_backgroundSprite.setSize(size);
	spriteBatch->drawSprite(m_backgroundSprite);
	
	for(int y = 0; y < 3; ++y)
	{
		for(int x = 0; x < 3; ++x)
		{
			m_itemSlotSprite.setPosition(position + Vector2F(8.f + x * 48.f, 7.f + y * 48.f));
			spriteBatch->drawSprite(m_itemSlotSprite);
			m_storage.getSlotAt(x + y * 3)->drawItem(position + Vector2F(8.f + x * 48.f, 7.f + y * 48.f), spriteBatch, m_font);
		}
	}

	//spriteBatch->getGraphicsContext().drawRectangle(500, 50, 300, 300);
}

void Crafting::keyEvent(const KeyEvent & event)
{
	/*if(!event.isAction(KeyEvent::PRESS)) return;

	const VirtualKey key = event.getKey();
	if(key == XD_MOUSE_BUTTON_LEFT || key == XD_MOUSE_BUTTON_RIGHT)
	{
		Vector2 position = getPosition();
		for(uint y = 0; y < 3; ++y)
		{
			for(uint x = 0; x < 3; ++x)
			{
				Rect rect(position.x + 8.0f + x * 48.0f, position.y + 7.f + y * 48.0f, 42.0f, 42.0f);
				if(rect.contains(Input::getPosition()))
				{
					m_gameOverlay->performSlotAction(m_storage.getSlotAt(x + y * 3), key);
					break;
				}
			}
		}
	}*/
}