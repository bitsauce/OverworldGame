#include "AIController.h"
#include "Networking/Connection.h"
#include "Pawn.h"
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
		//setClientInputState(GLFW_RELEASE, i);
	}

	Pawn* closestPawn = nullptr;
	/*for(Pawn *pawn : m_world->getPawns())
	{
		if(pawn == m_pawn) continue;
		if(!closestPawn || (pawn->getCenter() - m_pawn->getCenter()).magnitude() < (closestPawn->getCenter() - m_pawn->getCenter()).magnitude())
		{
			closestPawn = pawn;
		}
	}

	if(closestPawn)
	{
		Vector2F playerPosition = closestPawn->getCenter(), position = m_pawn->getCenter();
		if(playerPosition.x < position.x)
		{
			setClientInputState(GLFW_PRESS, INPUT_MOVE_LEFT);
		}
		else
		{
			setClientInputState(GLFW_PRESS, INPUT_MOVE_RIGHT);
		}

		if(m_prevPos.x == m_pawn->getX())
		{
			setClientInputState(GLFW_PRESS, INPUT_JUMP);
		}
	}

	m_prevPos = m_pawn->getPosition();*/
}