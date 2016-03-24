#ifndef PLAYER_H
#define PLAYER_H

#include "Pawn.h"

class Player : public Pawn
{
public:
	Player(const string &name, InputManager *input, World *world, const bool local);
	~Player();

	static DynamicEntity *Factory(World *world)
	{
		return new Player("NULL", Game::GetInstance()->getInputManager(), world, false);
	}

	string getName() const
	{
		return m_name;
	}

private:
	const string m_name;
};

#endif // PLAYER_H