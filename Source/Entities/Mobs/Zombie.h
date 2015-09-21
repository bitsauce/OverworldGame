#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Config.h"
#include "Entities/Player.h"
#include "Entities/Humanoid.h"
#include "Entities/Physics/DynamicEntity.h"

class Game;

class Zombie : public Player
{
public:
	Zombie(Game *game);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

private:
};

#endif // ZOMBIE_H