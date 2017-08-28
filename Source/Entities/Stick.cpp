#include "Stick.h"
#include "Game/RayCast.h"

Stick::Stick(const Json::Value &attributes) :
	Entity(0, "Stick", attributes),
	m_sprite(Resource<Texture2D>("Sprites/Items/Stick")),
	m_hitState(false),
	m_deleteTime(0.0f),
	m_moveToAlpha(0.0f)
{
	// Set up sprite
	m_sprite.getTexture()->setFiltering(Texture2D::LINEAR);
	m_sprite.setRegion(TextureRegion(), true);
	m_sprite.setOrigin(m_sprite.getSize() * 0.5f);
	m_sprite.setPosition(getPosition());

	// Set angle, velocity and gravity scale
	Vector2F dir(1.0f, 0.0f); dir.rotate(attributes.get("angle", 0.0f).asFloat());
	setVelocity(dir * attributes.get("speed", 45.0f).asFloat());
	setGravityScale(0.75f);
}

void Stick::onDraw(DrawEvent *e)
{
	// TODO: Make a system for drawing lots of entites effectively
	SpriteBatch *spriteBatch = (SpriteBatch*)e->getUserData();
	m_sprite.setPosition(getDrawPosition(e->getAlpha()));
	spriteBatch->drawSprite(m_sprite);
}

void Stick::onTick(TickEvent *e)
{
	Entity::onTick(e);
}