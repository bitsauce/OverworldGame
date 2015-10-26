#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Config.h"
#include "../Pawn.h"
#include "../Humanoid.h"
#include "../DynamicEntity.h"

class OverworldGame;

class Zombie : public Pawn
{
public:
	Zombie(OverworldGame *game);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	static DynamicEntity *Factory(World * world)
	{
		return new Zombie(0);
	}
private:
};

#endif // ZOMBIE_H