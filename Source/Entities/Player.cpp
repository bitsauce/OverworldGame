#include "Constants.h"
#include "Player.h"
#include "Networking/Controller.h"
#include "Entities/EntityData.h"

Player::Player(World *world, const Json::Value &attributes) :
	Pawn(world, attributes),
	m_font(Resource<Font>("Fonts/Debug")),
	m_name(attributes.get("name", "<UNKNOWN>").asString())
{
}

Player::~Player()
{
}

void Player::onDraw(DrawEvent *e)
{
	Pawn::onDraw(e);

	// Draw player name
	SpriteBatch *spriteBatch = (SpriteBatch*)e->getUserData();
	m_font->draw(spriteBatch, getCenter(e->getAlpha()) - Vector2F(0, 64), m_name, FONT_ALIGN_CENTER);
}
