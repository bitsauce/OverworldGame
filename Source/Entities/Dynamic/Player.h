#pragma once

#include "Pawn.h"

class Player : public Pawn
{
public:
	Player(World *world, const bool local);
	~Player();

	static DynamicEntity *Factory(World *world)
	{
		return new Player(world, false);
	}
protected:
};