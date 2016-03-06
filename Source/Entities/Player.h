#ifndef PLAYER_H
#define PLAYER_H

#include "Pawn.h"

class Player : public Pawn
{
public:
	Player(InputManager *input, World *world, const bool local);
	~Player();

	static DynamicEntity *Factory(World *world)
	{
		return new Player(Game::GetInstance()->getInputManager(), world, false);
	}
};

#endif // PLAYER_H