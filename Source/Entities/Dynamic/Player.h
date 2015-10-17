#pragma once

#include "Pawn.h"

class Player : public Pawn
{
public:
	Player(Game *game, const bool local);
	~Player();

	static DynamicEntity *Factory(World *world)
	{
		return new Player(0, false);
	}
protected:
};