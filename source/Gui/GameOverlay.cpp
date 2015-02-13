#include "GameOverlay.h"
#include "Game/ItemContainer.h"
#include "Game/Player.h"
#include "Items/Items.h"

GameOverlay::GameOverlay(Player *player, UiObject *parent) :
	UiObject(parent),
	m_player(player),
	m_spriteItemSlot(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlot.png")),
	m_spriteItemSlotSelected(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlotSelected.png")),
	m_font(ResourceManager::get<Font>("Consolas 10"))
{
	m_spriteItemSlot.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_spriteItemSlot.setDepth(-1.f);
	m_spriteItemSlotSelected.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_spriteItemSlotSelected.setDepth(-1.f);
}

void GameOverlay::update()
{
}

void GameOverlay::draw(SpriteBatch *spriteBatch)
{
	ItemContainer &itemContainer = m_player->getItemContainer();
	for(uint i = 0; i < itemContainer.getSize(); ++i)
	{
		Sprite &sprite = i == m_player->getSelectedItemSlot() ? m_spriteItemSlotSelected : m_spriteItemSlot;
		sprite.setPosition(5.f + i * 34.f, 5.f);
		spriteBatch->drawSprite(sprite);
		ItemData *item;
		if((item = ItemData::get(itemContainer.getItemAt(i))) != nullptr)
		{
			spriteBatch->drawSprite(Sprite(item->getIconTexture(), Rect(5.f + i * 34.f, 5.f, 32.f, 32.f)));
		}
		spriteBatch->drawText(Vector2(7.f + i * 34.f, 7.f), util::intToStr(itemContainer.getItemAmountAt(i)), m_font);
	}
}