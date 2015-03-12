#ifndef BLOCK_ENTITY_H
#define BLOCK_ENTITY_H

#include "Config.h"

class Thing
{
public:
	Thing(const int x, const int y);

	virtual void draw(SpriteBatch *spriteBatch, const float alpha) { }

private:
	const int m_x, m_y;
};

#endif // BLOCK_ENTITY_H