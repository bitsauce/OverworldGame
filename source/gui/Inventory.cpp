#include "Inventory.h"
#include "Game/ItemContainer.h"
#include "Game/Player.h"

GameOverlay::GameOverlay(Player *player, UiObject *parent) :
	UiObject(parent),
	m_player(player),
	m_spriteItemSlot(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlot.png")),
	m_spriteItemSlotSelected(ResourceManager::get<Texture2D>(":/Sprites/Inventory/ItemSlotSelected.png"))
{
	m_spriteItemSlot.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
	m_spriteItemSlotSelected.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
}

void GameOverlay::update()
{
}

void GameOverlay::draw(SpriteBatch *spriteBatch)
{
	for(uint i = 0; i < m_player->getItemContainer().getSize(); ++i)
	{
		Sprite &sprite = i == m_player->getSelectedItemSlot() ? m_spriteItemSlotSelected : m_spriteItemSlot;
		sprite.setPosition(5 + i * 34, 5);
		spriteBatch->drawSprite(sprite);
	}
}