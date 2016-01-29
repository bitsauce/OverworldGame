#include "Inventory.h"
#include "Game/Game.h"
#include "GameOverlay.h"
#include "Items/ItemData.h"
#include "Game/Storage.h"
#include "Entities/Player.h"

Inventory::Inventory(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(gameOverlay),
	m_gameOverlay(gameOverlay),
	m_itemSlotSprite(Game::GetInstance()->getResourceManager()->get<Texture2D>(":/Sprites/Inventory/ItemSlot.png").get()),
	m_backgroundSprite(Game::GetInstance()->getResourceManager()->get<Texture2D>(":/Sprites/Inventory/Inventory.png").get()),
	m_font(Game::GetInstance()->getResourceManager()->get<Font>(UI_INVENTORY_FONT)),
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

	setAnchor(Vector2F(0.5f, 0.85f));
	setSize(Vector2F(970.0f, 248.0f)/gameOverlay->getSize());
	setPosition(Vector2F(0.0f));
}

void Inventory::toggle(int action)
{
	//if(action != GLFW_PRESS) return;
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

void Inventory::onTick(TickEvent *e)
{
	//setSize(Vector2F(970.0f, 248.0f)/m_parent->getSize());

	m_animationTime += m_showing ? e->getDelta() : -e->getDelta();
	if(m_animationTime > m_animationDuration)
	{
		m_animationTime = m_animationDuration;
	}
	else if(m_animationTime < 0.0f)
	{
		m_animationTime = 0.0f;
	}

	m_interpolateStartY = getRect().position.y;
	m_interpolateEndY = math::lerp(getRect().size.y + 0.15f, 0.0f, m_animationTime/m_animationDuration);
	m_interpolateStartAlpha = m_fadeInAlpha;
	m_interpolateEndAlpha = math::lerp(0.0f, 1.0f, m_animationTime/m_animationDuration);
	
	UiObject::onTick(e);
}

void Inventory::onDraw(DrawEvent *e)
{
	RectF rect = getRect();
	rect.position.y = math::lerp(m_interpolateStartY, m_interpolateEndY, e->getAlpha());
	setRect(rect);
	m_fadeInAlpha = math::lerp(m_interpolateStartAlpha, m_interpolateEndAlpha, e->getAlpha());

	if(m_animationTime == 0.0f) return;

	Color fadeColor(255, 255, 255, (uchar) (255 * m_fadeInAlpha));
	m_backgroundSprite.setColor(fadeColor);
	m_itemSlotSprite.setColor(fadeColor);

	Vector2F position = getDrawPosition();
	Vector2F size = getDrawSize();
	
	m_backgroundSprite.setSize(size); // TODO: Remove once the sprite is of correct size

	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();

	m_backgroundSprite.setPosition(position);
	spriteBatch->drawSprite(m_backgroundSprite);

	Bag *bag = m_gameOverlay->getPlayer()->getBag();
	for(uint y = 0; y < bag->getHeight(); ++y)
	{
		for(uint x = 0; x < bag->getWidth(); ++x)
		{
			m_itemSlotSprite.setPosition(position + Vector2F(8.f + x * 48.f, 7.f + y * 48.f));
			spriteBatch->drawSprite(m_itemSlotSprite);

			bag->getStorage()->getSlotAt(x + y * bag->getWidth())->drawItem(position + Vector2F(8.f + x * 48.f, 7.f + y * 48.f), spriteBatch, m_font);
		}
	}
}

void Inventory::keyEvent(const KeyEvent & event)
{
	/*if(!event.isAction(KeyEvent::PRESS)) return;

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
	}*/
}