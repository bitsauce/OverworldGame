#include "Zombie.h"
#include "Game/Game.h"
#include "Entities/Pawn.h"
#include "Entities/AIController.h"
#include "Constants.h"

Zombie::Zombie(Game *game) :
	Pawn(game, ENTITY_ZOMBIE)
{
	//setBodyPart(HEAD, ResourceManager::get<Texture2D>(":/Sprites/Mobs/Zombie/Head.png")->getPixmap());
	setController(new AIController(game->getWorld()));
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