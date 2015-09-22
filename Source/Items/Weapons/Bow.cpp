#include "Bow.h"
#include "Constants.h"
#include "Entities/Pawn.h"
#include "Entities/Physics/DynamicEntity.h"
#include "World/Camera.h"
#include "World/World.h"
#include "Game/Game.h"
#include "Game/RayCast.h"

Bow::Bow(Game *game) :
	m_game(game),
	m_bowSprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Weapons/WoodenBow.png"))
{
	m_bowSprite.setRegion(TextureRegion(), true);
}

class Arrow : public DynamicEntity
{
public:
	Arrow(Pawn *owner, World *world, const Vector2 &pos, const Vector2 &dir, const float speed) :
		DynamicEntity(world, ENTITY_ARROW),
		m_owner(owner),
		m_sprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Weapons/Arrow.png")),
		m_hasHit(false),
		m_deleteTime(0.0f)
	{
		m_sprite.getTexture()->setFiltering(Texture2D::LINEAR);
		m_sprite.setRegion(TextureRegion(), true);
		m_sprite.setOrigin(m_sprite.getSize() * 0.5f);

		m_prevPosition = pos - m_sprite.getSize() * 0.5f,
		setPosition(m_prevPosition);
		setVelocity(dir.normalized() * speed);
		setGravityScale(1.0f);
		m_angle = m_prevAngle = atan2(dir.y, dir.x);
	}

	void draw(SpriteBatch *spriteBatch, const float alpha)
	{
		float angle = math::lerp(m_prevAngle, m_angle, alpha);
		m_sprite.setRotation(angle * (180.0f / PI));
		m_sprite.setPosition(math::lerp(m_prevPosition, getPosition(), alpha));

		/*Vector2 aabb[4];
		m_sprite.getAABB(aabb);

		Vector2 dt = getPosition() - m_prevPosition;
		Vector2 pos = (aabb[1] + aabb[2]) / 2.0f;

		GraphicsContext &context = spriteBatch->getGraphicsContext();
		context.setModelViewMatrix(spriteBatch->getState().projectionMatix);
		{
			RayCast rayCast(bind(&Arrow::plotTest, this, placeholders::_1, placeholders::_2));
			rayCast.trace(pos, pos + dt);
			for(Vector2i p : rayCast.getPoints())
			{
				context.drawRectangle(p, Vector2(1.0f), Color(255, 0, 0, 127));
			}
		}*/

		spriteBatch->drawSprite(m_sprite);
	}

	bool plotTest(int x, int y)
	{
		return !m_world->getTerrain()->isBlockAt(floor(x / BLOCK_PXF), floor(y / BLOCK_PXF), TERRAIN_LAYER_MIDDLE);
	}

	void update(const float delta)
	{
		m_prevPosition = getPosition();
		if(m_hasHit)
		{
			//m_deleteTime += delta;
			//if(m_deleteTime > 10.0f)
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

		Vector2 dt = getPosition() - m_prevPosition;
		Vector2 pos = (aabb[1] + aabb[2]) / 2.0f;

		RayCast rayCast(bind(&Arrow::plotTest, this, placeholders::_1, placeholders::_2));
		if(!m_hasHit && rayCast.trace(pos, pos + dt))
		{
			m_hasHit = true;
			setPosition(m_prevPosition);
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

private:
	Pawn *m_owner;
	Sprite m_sprite;
	Vector2 m_prevPosition;
	float m_angle;
	float m_prevAngle;
	bool m_hasHit;
	float m_deleteTime;
};

void Bow::use(Pawn *player, const float delta)
{
	if(player->getStorage()->removeItem(ITEM_ARROW) == 0)
	{
		new Arrow(player, m_game->getWorld(), player->getCenter(), m_game->getWorld()->getCamera()->getInputPosition() - player->getCenter(), 50.0f);
	}
}

void Bow::draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha)
{
	player->getHumanoid().drawRightHandSprite(m_bowSprite, Vector2(10.0f, 24.0f), spriteBatch);
}