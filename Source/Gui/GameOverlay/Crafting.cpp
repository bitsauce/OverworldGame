#include "Crafting.h"
#include "Gui/GameOverlay.h"
#include "Game/Game.h"
#include "Items/ItemData.h"

Crafting::Crafting(Scene *scene, GameOverlay *gameOverlay) :
	UiObject(scene, gameOverlay),
	m_gameOverlay(gameOverlay),
	m_itemSlotSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlot.png")),
	m_backgroundSprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/Inventory.png")),
	m_itemContainer(9)
{
	m_itemSlotSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_itemSlotSprite.setDepth(-1.f);
	m_backgroundSprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_backgroundSprite.setDepth(-3.f);

	setAnchor(Vector2(0.75f, 0.25f));
	setSize(Vector2(154.0f, 152.0f)/m_parent->getSize());
	setPosition(Vector2(0.0f));
}

void Crafting::update(float delta)
{
	if(!m_gameOverlay->isCrafting()) return;

	setSize(Vector2(154.0f, 152.0f)/m_parent->getSize());
}

void Crafting::draw(SpriteBatch *spriteBatch, float alpha)
{
	if(!m_gameOverlay->isCrafting()) return;

	Vector2 position = getPosition();
	Vector2 size = getSize();

	m_backgroundSprite.setPosition(position);
	m_backgroundSprite.setSize(size);
	spriteBatch->drawSprite(m_backgroundSprite);
	
	for(int y = 0; y < 3; ++y)
	{
		for(int x = 0; x < 3; ++x)
		{
			m_itemSlotSprite.setPosition(position + Vector2(8.f + x * 48.f, 7.f + y * 48.f));
			spriteBatch->drawSprite(m_itemSlotSprite);

			ItemContainer::Slot &slot = m_itemContainer.getSlotAt(x + y * 3);
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

	//spriteBatch->getGraphicsContext().drawRectangle(500, 50, 300, 300);
}