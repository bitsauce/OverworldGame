#ifndef PLAYER_H
#define PLAYER_H

#include "Pawn.h"
#include "Game/Game.h"

class Player : public Pawn
{
public:
	Player(const string &name, OverworldGame *game, const bool local);
	~Player();

	static DynamicEntity *Factory(World*)
	{
		return new Player("NULL", (OverworldGame*) Game::GetInstance(), false);
	}

	string getName() const
	{
		return m_name;
	}

private:
	const string m_name;
};

#endif // PLAYER_H