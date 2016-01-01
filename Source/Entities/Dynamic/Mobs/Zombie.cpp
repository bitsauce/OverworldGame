#include "Zombie.h"
#include "Game/Game.h"
#include "Entities/Dynamic/Pawn.h"
#include "Entities/Dynamic/AIController.h"
#include "Constants.h"

Zombie::Zombie(World *world) :
	Pawn(world, ENTITY_ZOMBIE)
{
	setController(new AIController(world));
	m_moveSpeed = 2.5f;
}

void Zombie::onTick(TickEvent *e)
{
	Pawn::update(delta);
}

void Zombie::onDraw(DrawEvent *e)
{
	Pawn::draw(spriteBatch, alpha);
}