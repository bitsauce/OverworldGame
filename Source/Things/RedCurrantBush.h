#ifndef RED_CURRANT_BUSH_H
#define RED_CURRANT_BUSH_H

#include "Config.h"
#include "Thing.h"

class RedCurrantBush : public Thing
{
	friend class ThingData;
public:
	RedCurrantBush(const int x, const int y);

	void draw(SpriteBatch *spriteBatch);

private:
	Sprite m_sprite;
	float m_growTimer;

	static void Factory(const int x, const int y) { new RedCurrantBush(x, y); }
};

#endif // RED_CURRANT_BUSH_H