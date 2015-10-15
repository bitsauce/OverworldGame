#include "Constants.h"
#include "Player.h"
#include "PlayerController.h"

Player::Player(Game *game, const bool local) :
	Pawn(game, ENTITY_PLAYER)
{
	// Create player controller
	setController(new PlayerController(local));
}

Player::~Player()
{
	// Delete controller
	delete m_controller;
}