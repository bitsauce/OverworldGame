#include "Zombie.h"
#include "World/World.h"
#include "Entities/Player.h"
#include "Constants.h"

Zombie::Zombie(Game *game) :
	Player(game)
{
	// Set body size
	Entity::setSize(24, 48);

	//setBodyPart(HEAD, ResourceManager::get<Texture2D>(":/Sprites/Mobs/Zombie/Head.png")->getPixmap());
}

#define SGN(x) ((x < 0) ? -1 : (x > 0))

void Zombie::update(const float delta)
{
	Player::update(delta);
}

void Zombie::draw(SpriteBatch *spriteBatch, const float alpha)
{
	Player::draw(spriteBatch, alpha);
}