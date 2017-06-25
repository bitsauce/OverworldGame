#include "Stick.h"
#include "Game/RayCast.h"

/*Stick::Stick(Pawn *owner, World *world, const Vector2F &pos, const Vector2F &dir, const float speed) :
	Entity(world, ENTITY_STICK),
	m_owner(owner),
	m_sprite(Resource<Texture2D>("Sprites/Items/Weapons/Arrow")),
	m_hitState(false),
	m_deleteTime(0.0f),
	m_moveToAlpha(0.0f)
{
	// Set up sprite
	m_sprite.getTexture()->setFiltering(Texture2D::LINEAR);
	m_sprite.setRegion(TextureRegion(), true);
	m_sprite.setOrigin(m_sprite.getSize() * 0.5f);

	// Set position, angle, velocity and gravity scale
	setPosition(pos);
	setVelocity(dir.normalized() * speed);
	setGravityScale(0.75f);
}

void Stick::onDraw(DrawEvent *e)
{
	// TODO: Make a system for drawing lots of entites effectively
	SpriteBatch *spriteBatch = (SpriteBatch*)e->getUserData();
	m_sprite.setPosition(getPosition());
	spriteBatch->drawSprite(m_sprite);
}

void Stick::onTick(TickEvent *e)
{
}*/