#ifndef BLOCK_ENTITY_H
#define BLOCK_ENTITY_H

#include "Config.h"

class Player;

class Thing
{
public:
	Thing(const int x, const int y);

	virtual void activate(Player *player) {}

	virtual void draw(SpriteBatch *spriteBatch, const float alpha) {}

	int getX() const { return m_x; }
	int getY() const { return m_y; }

private:
	const int m_x, m_y;
};

#endif // BLOCK_ENTITY_H