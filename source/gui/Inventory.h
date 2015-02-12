#ifndef INVENTORY_H
#define INVENTORY_H

#include "Config.h"
#include "UiObject.h"

class ItemContainer;
class Player;

class GameOverlay : public UiObject, public MouseListener
{
public:
	GameOverlay(Player *player, UiObject *object);

	void update();
	void draw(SpriteBatch *spriteBatch);

private:

	Player *m_player;
	Sprite m_spriteItemSlot;
	Sprite m_spriteItemSlotSelected;
};

#endif // INVENTORY_H