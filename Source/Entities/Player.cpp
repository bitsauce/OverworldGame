#include "Constants.h"
#include "Player.h"
#include "PlayerController.h"

Player::Player(const string &name, OverworldGame *game, const bool local) :
	Pawn(game->getWorld(), ENTITY_PLAYER),
	m_name(name),
	m_font(Game::GetInstance()->getResourceManager()->get<Font>("Fonts/Debug"))
{
	// Create player controller
	//setController(new PlayerController(game, local));
}

Player::~Player()
{
	// Delete controller
	delete m_controller;
}

void Player::onDraw(DrawEvent *e)
{
	SpriteBatch *spriteBatch = (SpriteBatch*)e->getUserData();
	//m_font->draw(spriteBatch, getPosition() - Vector2F(16, 48), m_name);
	Pawn::onDraw(e);
}
