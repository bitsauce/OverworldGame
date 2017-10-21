#include "Bow.h"
#include "Constants.h"
#include "Entities/Pawn.h"
#include "Entities/Arrow.h"
#include "World/World.h"
#include "Game/Game.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Animation/RegionAttachment.h"

Bow::Bow(World *world, Pawn *pawn, const Json::Value &attributes) :
	Item(world, pawn),
	m_attachTexture(Resource<Texture2D>("Sprites/Items/Weapons/WoodenBow")),
	m_sprite(Resource<Texture2D>("Sprites/Items/Weapons/WoodenBowAnim")),
	m_charging(false),
	m_chargeTime(0.0f),
	m_chargeAnim(1, 4)
{
	m_sprite.getTexture()->setFiltering(Texture2D::LINEAR);
	m_sprite.setRegion(m_chargeAnim.getKeyFrame(0), true);
	m_sprite.setDepth(-1.0f);
}

void Bow::equip()
{
	RegionAttachment *attachment = m_pawn->getHumanoid().setAttachment(Humanoid::RIGHT_HAND, "Right_Hand_Equip", "Bow");
	attachment->setPosition(-3.5f, 0.0f);
	attachment->setRotation(0.0f);
}

void Bow::unequip()
{
	m_pawn->getHumanoid().clearAttachment(Humanoid::RIGHT_HAND);
}

void Bow::update(const float delta)
{
	const Vector2F aimCenter = m_pawn->getCenter() - Vector2F(0.0f, 12.0f);
	const Vector2F dir = Vector2F(m_pawn->getController()->getInputPosition() - aimCenter).normalized();
	if(m_pawn->getController()->getInputState(Controller::INPUT_USE_ITEM))
	{
		m_charging = true;
		m_chargeTime += delta * 3;
		if(dir.x > 0.0f)
		{
			m_pawn->getHumanoid().getSkeleton()->setFlipX(false);
			if(dir.y > 0.0f)
			{
				m_pawn->getHumanoid().setPostBlendAnimations(Humanoid::ANIM_ARROW_AIM_FW, Humanoid::ANIM_ARROW_AIM_DW, dir.angle() / (PI / 2));
			}
			else
			{
				m_pawn->getHumanoid().setPostBlendAnimations(Humanoid::ANIM_ARROW_AIM_FW, Humanoid::ANIM_ARROW_AIM_UP, -dir.angle() / (PI / 2));
			}
		}
		else
		{
			m_pawn->getHumanoid().getSkeleton()->setFlipX(true);
			if(dir.y > 0.0f)
			{
				m_pawn->getHumanoid().setPostBlendAnimations(Humanoid::ANIM_ARROW_AIM_FW, Humanoid::ANIM_ARROW_AIM_DW, 2.0f - dir.angle() / (PI / 2));
			}
			else
			{
				m_pawn->getHumanoid().setPostBlendAnimations(Humanoid::ANIM_ARROW_AIM_FW, Humanoid::ANIM_ARROW_AIM_UP, 2.0f + dir.angle() / (PI / 2));
			}
		}
		m_pawn->getHumanoid().clearAttachment(Humanoid::RIGHT_HAND);
		m_pawn->getHumanoid().setAppearance(Humanoid::EYES, 0, "Angry");
		m_pawn->getHumanoid().setAppearance(Humanoid::MOUTH, 0, "Angry");
	}
	else if(m_charging)
	{
		if(m_pawn->getStorage()->removeItem(ItemData::GetByName("Arrow")->getID()) == 0)
		{
			Json::Value attributes;
			attributes["angle"] = dir.angle();
			attributes["speed"] = 45.0f * min(m_chargeTime, 1.0f);
			attributes["position"]["x"] = aimCenter.x;
			attributes["position"]["y"] = aimCenter.y;
			m_world->getConnection()->createEntity<Arrow>(attributes);
		}
		m_chargeTime = 0.0f;
		m_charging = false;
		m_pawn->getHumanoid().setPostBlendAnimations(Humanoid::ANIM_NULL, Humanoid::ANIM_NULL, 0.0f);

		RegionAttachment *attachment = m_pawn->getHumanoid().setAttachment(Humanoid::RIGHT_HAND, "Right_Hand_Equip", "Bow");
		attachment->setPosition(-3.5f, 0.0f);
		attachment->setRotation(0.0f);
		m_pawn->getHumanoid().setAppearance(Humanoid::EYES, 0, "Default");
		m_pawn->getHumanoid().setAppearance(Humanoid::MOUTH, 0, "Default");
	}
}

void Bow::draw(SpriteBatch *spriteBatch, const float alpha)
{
	if(m_charging)
	{
		TextureRegion region = m_chargeAnim.getKeyFrame(int(min(m_chargeTime + 0.15f, 1.0f) * 3));
		m_sprite.setRegion(region);

		Skeleton *skeleton = m_pawn->getHumanoid().getSkeleton();

		float angle = skeleton->findBone("Left_Hand")->getWorldRotation();
		if(skeleton->getFlipX())
		{
			m_sprite.setScaleX(-1.0f);
		}
		else
		{
			m_sprite.setScaleX(1.0f);
			angle *= -1;
		}

		Vector2F pos = skeleton->getPosition() + skeleton->findBone("Left_Hand")->getWorldPosition();
		m_sprite.setPosition(pos);
		m_sprite.setOrigin(m_sprite.getSize() * 0.5f);
		m_sprite.setRotation(angle);
		spriteBatch->drawSprite(m_sprite);
	}
}