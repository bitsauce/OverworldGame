#pragma once

#include "Pawn.h"

class Player : public Pawn
{
public:
	Player(Game *game, const bool local);
	~Player();

	static Entity *Factory(Game *game)
	{
		return new Player(game, false);
	}
protected:
};