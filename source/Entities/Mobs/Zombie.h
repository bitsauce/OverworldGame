#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Entities/Entity.h"
#include "Entities/Humanoid.h"
#include "Physics/PhysicsBody.h"

class Zombie : public Entity
{
public:
	Zombie(World &world);

	void update();
	void draw(SpriteBatch *spriteBatch);

	PhysicsBody &getBody() { return m_body; }
private:
	// Human model
	Humanoid m_humanoid;
	
	// Physics body
	PhysicsBody m_body;
};

#endif // ZOMBIE_H