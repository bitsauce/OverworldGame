#ifndef TORCH_H
#define TORCH_H

#include "Config.h"
#include "Thing.h"
#include "Lighting/Spotlight.h"

class Torch : public Thing
{
	friend class ThingData;
public:
	Torch(const int x, const int y);

	void draw(SpriteBatch *spriteBatch);

private:
	Sprite m_sprite;
	Spotlight m_spotlight;
	//float durration;?

	static void Factory(const int x, const int y) { new Torch(x, y); }
};

#endif // TORCH_H