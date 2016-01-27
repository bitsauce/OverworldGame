#pragma once

#include "Config.h"
#include "Controller.h"
#include "Networking/NetworkObject.h"
#include "DynamicEntity.h"

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