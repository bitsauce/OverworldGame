#include "Bow.h"
#include "Constants.h"
#include "Entities/Dynamic/Pawn.h"
#include "Entities/Dynamic/Arrow.h"
#include "World/Camera.h"
#include "World/World.h"
#include "Game/Game.h"
#include "Animation/Skeleton.h"

Bow::Bow(OverworldGame * game) :
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
		TextureRegion region = m_chargeAnim.getKeyFrame((int) min(m_chargeTime + 0.15f, 1.0f) * 3);
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