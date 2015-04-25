#ifndef TORCH_H
#define TORCH_H

#include "Config.h"
#include "Thing.h"
#include "World/Lighting/Spotlight.h"

class World;

class Torch : public Thing
{
	friend class ThingData;
public:
	Torch(World *world, const int x, const int y);

	void activate();

	void draw(SpriteBatch *spriteBatch, const float alpha);

private:
	World *m_world;
	Sprite m_sprite;
	Spotlight m_spotlight;
	//float durration;?

	static Thing *Factory(World *world, const int x, const int y) { return new Torch(world, x, y); }
};

#endif // TORCH_H