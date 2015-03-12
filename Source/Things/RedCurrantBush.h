#ifndef RED_CURRANT_BUSH_H
#define RED_CURRANT_BUSH_H

#include "Config.h"
#include "Thing.h"

class World;

class RedCurrantBush : public Thing
{
	friend class ThingData;
public:
	RedCurrantBush(const int x, const int y);

	void draw(SpriteBatch *spriteBatch);

private:
	Sprite m_sprite;
	float m_growTimer;

	static Thing *Factory(World &, const int x, const int y) { return new RedCurrantBush(x, y); }
};

#endif // RED_CURRANT_BUSH_H