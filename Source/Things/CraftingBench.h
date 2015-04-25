#ifndef CRAFTING_BENCH_H
#define CRAFTING_BENCH_H

#include "Config.h"
#include "Thing.h"

class World;

class CraftingBench : public Thing
{
	friend class ThingData;
public:
	CraftingBench(const int x, const int y);

	void activate(Player *player);
	
	void draw(SpriteBatch *spriteBatch, const float alpha);

private:
	Sprite m_sprite;

	static Thing *Factory(World*, const int x, const int y) { return new CraftingBench(x, y); }
};

#endif // CRAFTING_BENCH_H