#ifndef AMMO_H
#define AMMO_H

#include "Items/ItemData.h"

class OverworldGame;

class Ammo : public ItemData
{
public:
	Ammo(OverworldGame*);
	
	void use(Pawn *player, const float delta) { }
	void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha) { }
};

#endif // AMMO_H