#ifndef RED_CURRANT_BUSH_H
#define RED_CURRANT_BUSH_H

#include <x2d/x2d.h>

#include "blockentity.h"

class RedcurrantBush : public BlockEntity
{
public:
	RedcurrantBush();

	void moveEvent();
	void placeEvent();

	void draw(xd::SpriteBatch *SpriteBatch);

private:
	float m_growTimer;
	xd::Sprite *m_sprite;
};

#endif // RED_CURRANT_BUSH_H