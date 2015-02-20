#ifndef HOTBAR_SLOT_H
#define HOTBAR_SLOT_H

#include "Config.h"
#include "Gui/UiObject.h"

class Player;

class Hotbar : public UiObject
{
public:
	Hotbar(Player *player, UiObject *parent);

	void draw(SpriteBatch *spriteBatch);

private:
	Player *m_player;
	Sprite m_spriteItemSlot;
	Sprite m_spriteItemSlotSelected;
	Sprite m_spriteBackground;
};

#endif // HOTBAR_SLOT_H