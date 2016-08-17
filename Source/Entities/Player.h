#ifndef PLAYER_H
#define PLAYER_H

#include "Pawn.h"
#include "Game/Game.h"

class Player : public Pawn
{
public:
	Player(const string &name, OverworldGame *game, const bool local);
	~Player();

	static Entity *Factory(World*)
	{
		return new Player("NULL", (OverworldGame*) Game::GetInstance(), false);
	}

	string getName() const
	{
		return m_name;
	}


	virtual void onDraw(DrawEvent *e);

private:
	const string m_name;

	Resource<Font> m_font;
};

#endif // PLAYER_H