#include "Constants.h"
#include "Player.h"
#include "PlayerController.h"
#include "Entities/EntityData.h"

Player::Player(const Json::Value &attributes) :
	Pawn("Player", attributes),
	m_font(Resource<Font>("Fonts/Debug"))
{
	m_name = attributes.get("name", "<UNKNOWN>").asString();
}

Player::~Player()
{
	// Delete controller
	delete m_controller;
}

void Player::onDraw(DrawEvent *e)
{
	SpriteBatch *spriteBatch = (SpriteBatch*)e->getUserData();
	m_font->draw(spriteBatch, getDrawPosition(e->getAlpha()) - Vector2F(16, 48), m_name);
	Pawn::onDraw(e);
}
