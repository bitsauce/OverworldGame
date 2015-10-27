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

void Zombie::update(const float delta)
{
	Pawn::update(delta);
}

void Zombie::draw(SpriteBatch *spriteBatch, const float alpha)
{
	Pawn::draw(spriteBatch, alpha);
}