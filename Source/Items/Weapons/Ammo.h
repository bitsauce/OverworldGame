#ifndef AMMO_H
#define AMMO_H

#include "Items/ItemData.h"

class Game;

class Ammo : public ItemData
{
public:
	Ammo(Game *);
	
	void use(ItemContainer::Slot *slot, const float delta) { }
	void draw(Player *player, SpriteBatch *spriteBatch, const float alpha) { }
};

#endif // AMMO_H