#include "GameOverlay.h"
#include "Game/ItemContainer.h"
#include "Entities/Player.h"
#include "Gui/GameOverlay/Hotbar.h"

GameOverlay::GameOverlay(Player *player, UiObject *parent) :
	UiObject(parent),
	m_player(player)
{
	m_hotbar = new Hotbar(player, parent);
}

void GameOverlay::update()
{
}

void GameOverlay::draw(SpriteBatch *spriteBatch)
{
	/*ItemContainer &itemContainer = m_player->getItemContainer();
	for(uint i = 0; i < itemContainer.getSize(); ++i)
	{
		Sprite &sprite = i == m_player->getSelectedItemSlot() ? m_spriteItemSlotSelected : m_spriteItemSlot;
		sprite.setPosition(5.f + i * 34.f, 5.f);
		spriteBatch->drawSprite(sprite);
		ItemData *item;
		if((item = ItemData::get(itemContainer.getItemAt(i))) != nullptr)
		{
			spriteBatch->drawSprite(Sprite(item->getIconTexture(), Rect(13.f + i * 34.f, 13.f, 16.f, 16.f)));
		}
		spriteBatch->drawText(Vector2(7.f + i * 34.f, 7.f), util::intToStr(itemContainer.getItemAmountAt(i)), m_font);
	}*/
}