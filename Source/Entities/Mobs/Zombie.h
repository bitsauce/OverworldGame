#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Config.h"
#include "../Pawn.h"
#include "../Humanoid.h"
#include "../Entity.h"

class OverworldGame;

class Zombie : public Pawn
{
public:
	Zombie(World *world);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	static Zombie *Factory(World *world)
	{
		return new Zombie(world);
	}
private:
};

#endif // ZOMBIE_H