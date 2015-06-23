#include "Zombie.h"
#include "World/World.h"
#include "Entities/Player.h"
#include "Constants.h"

Zombie::Zombie(World *world) :
	DynamicEntity(world, ENTITY_ZOMBIE)
{
	// Set body size
	Entity::setSize(24, 48);

	//setBodyPart(HEAD, ResourceManager::get<Texture2D>(":/Sprites/Mobs/Zombie/Head.png")->getPixmap());
}

#define SGN(x) ((x < 0) ? -1 : (x > 0))

void Zombie::update(const float delta)
{
	Player *closestPlayer = nullptr;
	for(Player *player : m_world->getPlayers())
	{
		if(!closestPlayer || (player->getCenter() - getCenter()).magnitude() < (closestPlayer->getCenter() - getCenter()).magnitude())
		{
			closestPlayer = player;
		}
	}

	if(closestPlayer)
	{
		Vector2 playerPosition = closestPlayer->getCenter(), position = getCenter();
		setVelocityX(SGN(playerPosition.x - position.x) * 10.0f);
	}

	m_humanoid.update(delta);
	DynamicEntity::update(delta);
}

void Zombie::draw(SpriteBatch *spriteBatch, const float alpha)
{
	m_humanoid.draw(this, spriteBatch, 0.0f);
}