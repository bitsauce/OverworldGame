#include "Arrow.h"
#include "Game/RayCast.h"

Arrow::Arrow(Pawn *owner, World *world, const Vector2 &pos, const Vector2 &dir, const float speed) :
	DynamicEntity(world, ENTITY_ARROW),
	m_owner(owner),
	m_sprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Weapons/Arrow.png")),
	m_hasHit(false),
	m_deleteTime(0.0f)
{
	// Set up sprite
	m_sprite.getTexture()->setFiltering(Texture2D::LINEAR);
	m_sprite.setRegion(TextureRegion(), true);
	m_sprite.setOrigin(m_sprite.getSize() * 0.5f);

	// Set position, angle, velocity and gravity scale
	setPosition(pos - m_sprite.getSize() * 0.5f);
	setVelocity(dir.normalized() * speed);
	setGravityScale(0.75f);
	m_angle = m_prevAngle = atan2(dir.y, dir.x);
}

void Arrow::onDraw(DrawEvent *e)
{
	float angle = math::lerp(m_prevAngle, m_angle, alpha);
	m_sprite.setRotation(angle * (180.0f / PI));
	m_sprite.setPosition(getDrawPosition(alpha));
	spriteBatch->drawSprite(m_sprite);
}

bool Arrow::plotTest(int x, int y)
{
	return !m_world->getTerrain()->isBlockAt(floor(x / BLOCK_PXF), floor(y / BLOCK_PXF), WORLD_LAYER_MIDDLE);
}

void Arrow::onTick(TickEvent *e)
{
	if(m_hasHit)
	{
		m_deleteTime += delta;
		if(m_deleteTime > 10.0f)
		{
			delete this;
			return;
		}
		return;
	}

	m_allowRotation = true;
	if(!m_hasHit)
	{
		DynamicEntity::update(delta);
	}

	// Ray cast
	Vector2 aabb[4];
	m_sprite.getAABB(aabb);

	Vector2 dt = getPosition() - getLastPosition();
	Vector2 pos = (aabb[1] + aabb[2]) / 2.0f;

	RayCast rayCast(bind(&Arrow::plotTest, this, placeholders::_1, placeholders::_2));
	if(!m_hasHit && rayCast.trace(pos, pos + dt))
	{
		m_hasHit = true;
		moveTo(rayCast.getPoints().back() - m_sprite.getSize() * 0.5f);
	}

	/*
	// Add to testPlot
	for(Pawn *pawn : m_world->getPawns())
	{
	if(pawn == m_owner) continue;
	if(pawn->getRect().contains(Rect(m_sprite.getPosition(), m_sprite.getSize())))
	{
	m_hasHit = true;
	pawn->decHealth(100);
	m_deleteTime = 11.0f;
	}
	}*/

	m_prevAngle = m_angle;
	m_angle = m_hasHit ? (m_sprite.getRotation() / 180.0f * PI) : atan2(getVelocity().y, getVelocity().x);
}