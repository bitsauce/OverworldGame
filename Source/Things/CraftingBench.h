#ifndef CRAFTING_BENCH_H
#define CRAFTING_BENCH_H

#include "Config.h"
#include "Thing.h"

class Game;

class CraftingBench : public Thing
{
	friend class ThingData;
public:
	CraftingBench(Game *game, const int x, const int y);

	void activate(Pawn *player);
	
	void draw(SpriteBatch *spriteBatch, const float alpha);

private:
	Game *m_game;
	Sprite m_sprite;

	static Thing *Factory(Game *game, const int x, const int y) { return new CraftingBench(game, x, y); }
};

#endif // CRAFTING_BENCH_H