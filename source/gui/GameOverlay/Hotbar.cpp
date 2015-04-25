#include "Hotbar.h"
#include "Constants.h"
#include "Gui/Canvas.h"
#include "Game/ItemContainer.h"
#include "Items/ItemData.h"
#include "Entities/Player.h"

Hotbar::Hotbar(Scene *scene, Player *player, UiObject *parent) :
	UiObject(scene, parent),
	m_player(player),
	m_spriteItemSlot(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlot.png")),
	m_spriteItemSlotSelected(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlotSelected.png")),
	m_spriteBackground(ResourceManager::get<Texture2D>(":/Sprites/Inventory/Hotbar.png")),
	m_font(ResourceManager::get<Font>("Consolas 11"))
{
	m_spriteItemSlot.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_spriteItemSlot.setDepth(-1.f);
	m_spriteItemSlotSelected.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_spriteItemSlotSelected.setDepth(-1.f);
	m_spriteBackground.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_spriteBackground.setDepth(-2.f);

	m_font->setDepth(1.f);

	setAnchor(Vector2(0.5f, 0.98f));
	setSize(Vector2(490.f, 56.f)/parent->getSize());
	setPosition(Vector2(.0f));
}

void Hotbar::update(const float delta)
{
	setSize(Vector2(490.f, 56.f)/m_parent->getSize());
}

void Hotbar::draw(SpriteBatch *spriteBatch, const float alpha)
{
	// Get size and position
	Vector2 position = getPosition();
	Vector2 size = getSize();
	
	m_spriteBackground.setPosition(position);
	m_spriteBackground.setSize(size);
	spriteBatch->drawSprite(m_spriteBackground);

	ItemContainer &itemContainer = m_player->getItemContainer();
	for(uint i = 0; i < 10; ++i)
	{
		Sprite &sprite = i == m_player->getSelectedItemSlot() ? m_spriteItemSlotSelected : m_spriteItemSlot;
		sprite.setPosition(position + Vector2(8.f + i * 48.f, 7.f));
		spriteBatch->drawSprite(sprite);
		ItemData *item;
		if((item = ItemData::get(itemContainer.getItemAt(i))) != nullptr)
		{
			spriteBatch->drawSprite(Sprite(item->getIconTexture(), Rect(position.x + 13.f + i * 48.f, position.y + 12.f, 32.f, 32.f)));
		}
		spriteBatch->drawText(Vector2(position.x + 11.f + i * 48.f, position.y + 10.f), util::intToStr(itemContainer.getItemAmountAt(i)), m_font);
	}
}