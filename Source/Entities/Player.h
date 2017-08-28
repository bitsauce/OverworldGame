#ifndef PLAYER_H
#define PLAYER_H

#include "Pawn.h"
#include "Game/Game.h"

class Player : public Pawn
{
public:
	Player(World *world, const Json::Value &attributes);
	~Player();

	static Entity *Factory(const Json::Value &attributes)
	{
		return new Player(0, attributes);
	}

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