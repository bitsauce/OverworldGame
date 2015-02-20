#include "Hotbar.h"
#include "Game.h"
#include "Gui/Canvas.h"

Hotbar::Hotbar(Player *player, UiObject *parent) :
	UiObject(parent),
	m_player(player),
	m_spriteItemSlot(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlot.png")),
	m_spriteItemSlotSelected(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlotSelected.png")),
	m_spriteBackground(ResourceManager::get<Texture2D>(":/Sprites/Inventory/Hotbar.png"))
{
	m_spriteItemSlot.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true); m_spriteItemSlot.setSize(Vector2(42));
	m_spriteItemSlot.setDepth(-1.f);
	m_spriteItemSlotSelected.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true); m_spriteItemSlot.setSize(Vector2(42));
	m_spriteItemSlotSelected.setDepth(-1.f);
	m_spriteBackground.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_spriteBackground.setDepth(-2.f);

	setAnchor(Vector2(0.5f, 0.95f));
	setSize(Vector2(490.f, 56.f)/((Canvas*)canvas)->getCanvasSize());
	setPosition(Vector2(.0f));
}

void Hotbar::draw(SpriteBatch *spriteBatch)
{
	// Get size and position
	Vector2 position = getPosition();
	Vector2 size = getSize();
	
	m_spriteBackground.setPosition(position);
	m_spriteBackground.setSize(size);
	spriteBatch->drawSprite(m_spriteBackground);

	for(int i = 0; i < 10; ++i)
	{
		Sprite &sprite = i == m_player->getSelectedItemSlot() ? m_spriteItemSlotSelected : m_spriteItemSlot;
		sprite.setPosition(position + Vector2(8.f + i * 48.f, 7.f));
		spriteBatch->drawSprite(sprite);
	}
}