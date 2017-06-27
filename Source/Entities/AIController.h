#pragma once

#include "Config.h"
#include "Networking/Controller.h"

class World;

class AIController : public Controller
{
public:
	AIController(World *world);

	void update();

private:
	World *m_world;
	Vector2F m_prevPos;
};