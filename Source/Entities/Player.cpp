#include "Constants.h"
#include "Player.h"
#include "PlayerController.h"

Player::Player(const string &name, OverworldGame *game, const bool local) :
	Pawn(game->getWorld(), ENTITY_PLAYER),
	m_name(name)
{
	// Create player controller
	setController(new PlayerController(game, local));
}

Player::~Player()
{
	// Delete controller
	delete m_controller;
}