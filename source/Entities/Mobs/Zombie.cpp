#include "Zombie.h"
#include "World/World.h"
#include "Entities/Player.h"
#include "Constants.h"

Zombie::Zombie(World &world) :
	Entity(world, ENTITY_ZOMBIE),
	m_body(world)
{
	// Set body size
	m_body.setSize(24, 48);

	//setBodyPart(HEAD, ResourceManager::get<Texture2D>(":/Sprites/Mobs/Zombie/Head.png")->getPixmap());
}

#define SGN(x) ((x < 0) ? -1 : (x > 0))

void Zombie::update()
{
	Player *closestPlayer = nullptr;
	for(Player *player : m_world.getPlayers())
	{
		if(!closestPlayer || (player->getBody().getCenter() - m_body.getCenter()).magnitude() < (closestPlayer->getBody().getCenter() - m_body.getCenter()).magnitude())
		{
			closestPlayer = player;
		}
	}

	if(closestPlayer)
	{
		Vector2 playerPosition = closestPlayer->getBody().getCenter(), position = m_body.getCenter();
		m_body.setVelocityX(SGN(playerPosition.x - position.x) * 10.0f);
	}

	m_body.update();
	m_humanoid.update();
}

void Zombie::draw(SpriteBatch *spriteBatch)
{
	m_humanoid.draw(&m_body, spriteBatch);
}