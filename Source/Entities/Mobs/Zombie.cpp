#include "Zombie.h"
#include "World/World.h"
#include "Entities/Pawn.h"
#include "Constants.h"

Zombie::Zombie(Game *game) :
	Pawn(game, ENTITY_ZOMBIE)
{
	//setBodyPart(HEAD, ResourceManager::get<Texture2D>(":/Sprites/Mobs/Zombie/Head.png")->getPixmap());
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