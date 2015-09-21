#include "AIController.h"
#include "Networking/Connection.h"
#include "Player.h"
#include "World/World.h"

AIController::AIController(World *world) :
	m_world(world)
{
}

void AIController::update()
{
	// Reset all input
	for(uint i = 0; i < INPUT_COUNT; ++i)
	{
		setClientInputState(GLFW_RELEASE, i);
	}

	Player *closestPlayer = nullptr;
	for(Player *player : m_world->getPlayers())
	{
		if(player == m_player) continue;
		if(!closestPlayer || (player->getCenter() - m_player->getCenter()).magnitude() < (closestPlayer->getCenter() - m_player->getCenter()).magnitude())
		{
			closestPlayer = player;
		}
	}

	if(closestPlayer)
	{
		Vector2 playerPosition = closestPlayer->getCenter(), position = m_player->getCenter();
		if(playerPosition.x < position.x)
		{
			setClientInputState(GLFW_PRESS, INPUT_MOVE_LEFT);
		}
		else
		{
			setClientInputState(GLFW_PRESS, INPUT_MOVE_RIGHT);
		}

		if(m_prevPos.x == m_player->getX())
		{
			setClientInputState(GLFW_PRESS, INPUT_JUMP);
		}
	}

	m_prevPos = m_player->getPosition();
}