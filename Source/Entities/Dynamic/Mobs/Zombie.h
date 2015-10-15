#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Config.h"
#include "../Pawn.h"
#include "../Humanoid.h"
#include "../DynamicEntity.h"

class Game;

class Zombie : public Pawn
{
public:
	Zombie(Game *game);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	static Entity *Factory(Game *game)
	{
		return new Zombie(game);
	}
private:
};

#endif // ZOMBIE_H