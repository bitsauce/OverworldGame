#include "Constants.h"
#include "Player.h"
#include "PlayerController.h"

Player::Player(InputManager *input, World *world, const bool local) :
	Pawn(world, ENTITY_PLAYER)
{
	// Create player controller
	setController(new PlayerController(input, local));
}

Player::~Player()
{
	// Delete controller
	delete m_controller;
}