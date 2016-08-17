#include "Arrow.h"
#include "Game/RayCast.h"

Arrow::Arrow(Pawn *owner, World *world, const Vector2F &pos, const Vector2F &dir, const float speed) :
	Entity(world, ENTITY_ARROW),
	m_owner(owner),
	m_sprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Items/Weapons/Arrow")),
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

void Arrow::onDraw(DrawEvent *e)
{
	if(m_hitState == 0)
	{
		Vector2F velocity = math::lerp(m_prevVelocity, getVelocity(), e->getAlpha());
		float angle = atan2(velocity.y, velocity.x);

		m_sprite.setRotation(angle * (180.0f / PI));
		m_sprite.setPosition(getDrawPosition(e->getAlpha()));
	}
	else if(m_hitState == 1)
	{
		if(e->getAlpha() < m_moveToAlpha)
		{
			m_sprite.setPosition(getDrawPosition(min(m_moveToAlpha, e->getAlpha())));
		}
		else
		{
			m_hitState = 2;
		}
	}

	if(m_hitState == 2)
	{
		m_sprite.setPosition(getDrawPosition(m_moveToAlpha));
	}

	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	spriteBatch->drawSprite(m_sprite);
}

bool Arrow::plotTest(int x, int y)
{
	/*
	// Add to testPlot
	for(Pawn *pawn : m_world->getPawns())
	{
	if(pawn == m_owner) continue;
	if(pawn->getRect().contains(Rect(m_sprite.getPosition(), m_sprite.getSize())))
	{
	m_hitState = true;
	pawn->decHealth(100);
	m_deleteTime = 11.0f;
	}
	}*/

	// Remove destructable block entities
	BlockEntity *blockEntity = m_world->getTerrain()->getBlockEntityAt(floor(x / BLOCK_PXF), floor(y / BLOCK_PXF), WORLD_LAYER_MIDDLE);
	if(blockEntity && blockEntity->getData()->getID() == BLOCK_ENTITY_POT)
	{
		m_world->getTerrain()->removeBlockEntityAt(blockEntity->getX(), blockEntity->getY(), WORLD_LAYER_MIDDLE);
		return true;
	}

	return !m_world->getTerrain()->isBlockAt(floor(x / BLOCK_PXF), floor(y / BLOCK_PXF), WORLD_LAYER_MIDDLE);
}

void Arrow::onTick(TickEvent *e)
{
	m_allowRotation = true;
	if(m_hitState == 0)
	{
		m_prevVelocity = getVelocity();
		Entity::onTick(e);

		// Ray cast
		Vector2F aabb[4];
		m_sprite.getAABB(aabb);
		Vector2F dt = getPosition() - getLastPosition();
		Vector2F pos = (aabb[1] + aabb[2]) / 2.0f;
		RayCast rayCast(bind(&Arrow::plotTest, this, placeholders::_1, placeholders::_2));
		if(rayCast.trace(pos, pos + dt))
		{
			Vector2F hitPosition = rayCast.getPoints().back();
			m_moveToAlpha = (hitPosition - getLastPosition()).length() / dt.length();
			setPosition(getLastPosition());
			moveTo(hitPosition);
			m_hitState = 1;
		}
	}
	else
	{
		m_hitState = 2;
		m_deleteTime += e->getDelta();
		if(m_deleteTime > 10.0f)
		{
			delete this;
			return;
		}
		return;
	}
}