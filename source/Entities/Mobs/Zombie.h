#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Entities/Humanoid.h"

class Zombie : public Humanoid
{
public:
	Zombie();

	void update();
private:
};

#endif // ZOMBIE_H