#include "Zombie.h"

Zombie::Zombie()
{
	setBodyPart(HEAD, ResourceManager::get<Texture2D>(":/Sprites/Mobs/Zombie/Head.png")->getPixmap());
}

void Zombie::update()
{
	//chase(findClosesPlayer());
}