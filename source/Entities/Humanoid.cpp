#include "Humanoid.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"

Humanoid::Humanoid() :
	m_currentAnim(nullptr)
{
	// Load skeleton data
	m_skeleton = new Skeleton(":/sprites/characters/anim/skeleton.json", ":/sprites/characters/anim/skeleton.atlas", 1.0f);
	m_skeleton->getTexture()->setFiltering(Texture2D::LINEAR);
	m_skeleton->setFlipY(true); // Not sure why this is needed, oh well

	// Setup spine animations
	m_animationStateData = new AnimationStateData(m_skeleton);
	m_animationStateData->setMix("idle", "walk", 0.2f);
	m_animationStateData->setMix("walk", "idle", 0.5f);
	m_animationStateData->setMix("jump", "idle", 0.1f);
	m_animationStateData->setMix("walk", "jump", 0.1f);
	m_animationStateData->setMix("jump", "idle", 0.1f);
	m_animationStateData->setMix("idle", "jump", 0.2f);
	
	// Create spine animation states
	m_mainAnimationState = new AnimationState(m_animationStateData);
	//m_mainAnimationState->setEventCallback(animationEvent);
	m_mainAnimationState->setLooping(true);
	
	m_itemAnimationState = new AnimationState(m_animationStateData);
	m_itemAnimationState->setLooping(true);
}

string Humanoid::getBodyPartName(const BodyPart part)
{
	switch(part)
	{
	case HEAD: return "head";
	case NECK: return "neck";
	case TORSO: return "torso";
	case HIPS: return "hips";
	case THIGH_LEFT: return "lthigh";
	case THIGH_RIGHT: return "rthigh";
	case SHOULDER_RIGHT: return "rshoulder";
	case SHOULDER_LEFT: return "lshoulder";
	case ARM_RIGHT: return "rarm";
	case ARM_LEFT: return "larm";
	case LEG_RIGHT: return "rleg";
	case LEG_LEFT: return "lleg";
	default: return "null";
	}
}

// ANIMATIONS
void Humanoid::setMainAnimation(const string &name)
{
	// Get animation by name
	Animation *anim = m_skeleton->findAnimation(name);
	if(anim == nullptr)
	{
		LOG("Humanoid::setMainAnimation() - Animation '%s' does not exists.", name);
		return;
	}
		
	// Make sure this animation isn't current
	if(m_currentAnim != anim)
	{
		m_mainAnimationState->setAnimation(anim);
		m_currentAnim = anim;
	}
}

void Humanoid::setItemAnimation(Animation *anim)
{
	// Make sure this animation isn't current
	if(m_itemAnimation != anim)
	{
		if(anim != nullptr)
		{
			m_itemAnimationState->setAnimation(anim);
		}
		m_itemAnimation = anim;
	}
}

void Humanoid::setBodyPart(const BodyPart part, const Pixmap &pixmap)
{
	// Replace body parts/clothing
	TextureRegion region = m_skeleton->getTextureRegion(getBodyPartName(part));
	Texture2DPtr texture = m_skeleton->getTexture();
	uint x0 = region.uv0.x * texture->getWidth(), y0 = region.uv0.y * texture->getHeight(),
		x1 = region.uv1.x * texture->getWidth(), y1 = region.uv1.y * texture->getHeight();
	m_skeleton->getTexture()->updatePixmap(x0, y0, pixmap);
}

void Humanoid::update()
{
	// Update animations
	m_mainAnimationState->update(Graphics::getTimeStep());
	if(m_itemAnimation)
	{
		m_itemAnimationState->update(Graphics::getTimeStep());
	}
}

#include "physics/physicsbody.h"
#include "constants.h"

void Humanoid::draw(PhysicsBody *m_body, SpriteBatch *spriteBatch)
{
	// Set animations
	m_mainAnimationState->setTimeScale(math::abs(m_body->getVelocity().x) * 0.5f);
	if(m_body->isContact(SOUTH))
	{
		m_mainAnimationState->setLooping(true);
		if(m_body->getVelocity().x >= 0.01f)
		{
			setMainAnimation("walk");
			m_skeleton->setFlipX(false);
		}
		else if(m_body->getVelocity().x <= -0.01f)
		{
			setMainAnimation("walk");
			m_skeleton->setFlipX(true);
		}
		else
		{
			setMainAnimation("idle");
			m_body->setVelocityX(0.0f);
			m_mainAnimationState->setTimeScale(1.0f);
		}
	}
	else
	{
		if(m_body->isContact(WEST)/* >= 3*/) // TODO: I should check for a column of 3 rows of blocks instead of simlply one
		{
			m_skeleton->setFlipX(false);
			m_mainAnimationState->setLooping(false);
			m_mainAnimationState->setTimeScale(5.0f);
			setMainAnimation("wall-slide");
		}
		else if(m_body->isContact(EAST))
		{
			m_skeleton->setFlipX(true);
			m_mainAnimationState->setLooping(false);
			m_mainAnimationState->setTimeScale(5.0f);
			setMainAnimation("wall-slide");
		}
		else
		{
			m_mainAnimationState->setLooping(false);
			m_mainAnimationState->setTimeScale(1.0f);
			setMainAnimation("jump");
		}
	}

	// Draw skeleton
	m_skeleton->setPosition(m_body->getPosition() + Vector2(m_body->getSize().x*0.5f, 48.0f));
	m_skeleton->draw(spriteBatch->getGraphicsContext());
}

void Humanoid::drawRightHandSprite(Sprite &sprite, const Vector2 &origin, SpriteBatch *spriteBatch)
{
	float angle = m_skeleton->findBone("rarm")->getWorldRotation();
	Vector2 size = sprite.getSize();
	if(m_skeleton->getFlipX())
	{
		sprite.setSize(sprite.getSize() * Vector2(-1.0f, 1.0f));
	}
	else
	{
		angle *= -1;
	}
	sprite.setPosition(m_skeleton->getPosition() + m_skeleton->findBone("rarm")->getWorldPosition() - origin + Vector2(cos(angle * 0.0174532925f), sin(angle * 0.0174532925f)) * 10);
	sprite.setOrigin(origin);
	sprite.setRotation(angle);
	spriteBatch->drawSprite(sprite);
	sprite.setSize(size);
}