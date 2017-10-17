#ifndef PLAYER_H
#define PLAYER_H

#include "Pawn.h"
#include "Game/Game.h"

class Player : public Pawn
{
	DECLARE_ENTITY(Player)
public:
	string getName() const
	{
		return m_name;
	}

	virtual void onDraw(DrawEvent *e);

private:
	string m_name;
	Resource<Font> m_font;
};

#endif // PLAYER_H