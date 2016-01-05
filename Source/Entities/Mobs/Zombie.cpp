#include "Zombie.h"
#include "Game/Game.h"
#include "Entities/Pawn.h"
#include "Entities/AIController.h"
#include "Constants.h"

Zombie::Zombie(World *world) :
	Pawn(world, ENTITY_ZOMBIE)
{
	setController(new AIController(world));
	m_moveSpeed = 2.5f;
}

void Zombie::onTick(TickEvent *e)
{
	Pawn::onTick(e);
}

void Zombie::onDraw(DrawEvent *e)
{
	Pawn::onDraw(e);
}