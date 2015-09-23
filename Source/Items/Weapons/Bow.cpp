#include "Bow.h"
#include "Constants.h"
#include "Entities/Pawn.h"
#include "Entities/Physics/DynamicEntity.h"
#include "World/Camera.h"
#include "World/World.h"
#include "Game/Game.h"
#include "Game/RayCast.h"
#include "Animation/Skeleton.h"

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
		// Set up sprite
		m_sprite.getTexture()->setFiltering(Texture2D::LINEAR);
		m_sprite.setRegion(TextureRegion(), true);
		m_sprite.setOrigin(m_sprite.getSize() * 0.5f);

		// Set position, angle, velocity and gravity scale
		m_prevPosition = pos - m_sprite.getSize() * 0.5f,
		setPosition(m_prevPosition);
		setVelocity(dir.normalized() * speed);
		setGravityScale(0.75f);
		m_angle = m_prevAngle = atan2(dir.y, dir.x);
	}

	void draw(SpriteBatch *spriteBatch, const float alpha)
	{
		float angle = math::lerp(m_prevAngle, m_angle, alpha);
		m_sprite.setRotation(angle * (180.0f / PI));
		m_sprite.setPosition(math::lerp(m_prevPosition, getPosition(), alpha));
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

		Vector2 dt = getPosition() - m_prevPosition;
		Vector2 pos = (aabb[1] + aabb[2]) / 2.0f;

		RayCast rayCast(bind(&Arrow::plotTest, this, placeholders::_1, placeholders::_2));
		if(!m_hasHit && rayCast.trace(pos, pos + dt))
		{
			m_hasHit = true;
			setPosition(rayCast.getPoints().back() - m_sprite.getSize() * 0.5f);
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

Bow::Bow(Game *game) :
	m_game(game),
	m_attachTexture(ResourceManager::get<Texture2D>(":/Sprites/Items/Weapons/WoodenBow.png")),
	m_sprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Weapons/WoodenBowAnim.png")),
	m_charging(false),
	m_chargeTime(0.0f),
	m_chargeAnim(1, 4)
{
	m_sprite.getTexture()->setFiltering(Texture2D::LINEAR);
	m_sprite.setRegion(m_chargeAnim.getKeyFrame(0), true);
	m_sprite.setDepth(-1.0f);
}

void Bow::equip(Pawn *player)
{
	player->getHumanoid().setAttachmentTexture(Humanoid::ARM_LEFT, 1, m_attachTexture);
}

void Bow::unequip(Pawn *player)
{
	player->getHumanoid().setAttachmentTexture(Humanoid::ARM_LEFT, 1, 0);
}

void Bow::update(Pawn *pawn, const float delta)
{
	Vector2 dir = m_game->getWorld()->getCamera()->getInputPosition() - pawn->getCenter();
	if(pawn->getController()->getInputState(Controller::INPUT_USE_ITEM))
	{
		m_charging = true;
		m_chargeTime += delta * 3;
		if(dir.x > 0.0f)
		{
			float alpha;
			pawn->getHumanoid().getSkeleton()->setFlipX(false);
			if(dir.y > 0.0f)
			{
				alpha = dir.angle() / (PI / 2);
				pawn->getHumanoid().setPostBlendAnimations(Humanoid::ANIM_ARROW_AIM_FW, Humanoid::ANIM_ARROW_AIM_DW, alpha);
			}
			else
			{
				alpha = -dir.angle() / (PI / 2);
				pawn->getHumanoid().setPostBlendAnimations(Humanoid::ANIM_ARROW_AIM_FW, Humanoid::ANIM_ARROW_AIM_UP, alpha);
			}
		}
		else
		{
			float alpha;
			pawn->getHumanoid().getSkeleton()->setFlipX(true);
			if(dir.y > 0.0f)
			{
				alpha = 1.0f - (dir.angle() - (PI / 2)) / (PI / 2);
				pawn->getHumanoid().setPostBlendAnimations(Humanoid::ANIM_ARROW_AIM_FW, Humanoid::ANIM_ARROW_AIM_DW, alpha);
			}
			else
			{
				alpha = 1.0f - (-dir.angle() - (PI / 2)) / (PI / 2);
				pawn->getHumanoid().setPostBlendAnimations(Humanoid::ANIM_ARROW_AIM_FW, Humanoid::ANIM_ARROW_AIM_UP, alpha);
			}
		}
		pawn->getHumanoid().setAttachmentTexture(Humanoid::ARM_LEFT, 1, 0);
	}
	else if(m_charging)
	{
		if(pawn->getStorage()->removeItem(ITEM_ARROW) == 0)
		{
			new Arrow(pawn, m_game->getWorld(), pawn->getCenter(), dir, 45.0f * min(m_chargeTime, 1.0f));
		}
		m_chargeTime = 0.0f;
		m_charging = false;
		pawn->getHumanoid().setPostBlendAnimations(Humanoid::ANIM_NULL, Humanoid::ANIM_NULL, 0.0f);
		pawn->getHumanoid().setAttachmentTexture(Humanoid::ARM_LEFT, 1, m_attachTexture);
	}
}

#include "Animation/Bone.h"

void Bow::draw(Pawn *pawn, SpriteBatch *spriteBatch, const float alpha)
{
	if(m_charging)
	{
		TextureRegion region = m_chargeAnim.getKeyFrame(min(m_chargeTime + 0.15f, 1.0f) * 3);
		m_sprite.setRegion(region);

		Skeleton *skeleton = pawn->getHumanoid().getSkeleton();

		float angle = skeleton->findBone("lhand")->getWorldRotation();
		if(skeleton->getFlipX())
		{
			m_sprite.setScaleX(-1.0f);
		}
		else
		{
			m_sprite.setScaleX(1.0f);
			angle *= -1;
		}

		Vector2 pos = skeleton->getPosition() + skeleton->findBone("lhand")->getWorldPosition();
		m_sprite.setPosition(pos - m_sprite.getSize() * 0.5f);
		m_sprite.setOrigin(m_sprite.getSize() * 0.5f);
		m_sprite.setRotation(angle);
		spriteBatch->drawSprite(m_sprite);
	}
}