#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Entities/Humanoid.h"
#include "Entities/Physics/DynamicEntity.h"

class Zombie : public DynamicEntity
{
public:
	Zombie(World &world);

	void update(const float dt);
	void draw(SpriteBatch *spriteBatch, const float alpha);

private:
	// Human model
	Humanoid m_humanoid;
};

#endif // ZOMBIE_H