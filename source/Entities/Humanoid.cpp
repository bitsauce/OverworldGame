#include "Humanoid.h"
#include "Constants.h"

#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"

#include "Physics/PhysicsBody.h"

Humanoid::Humanoid() :
	m_preAnimation(nullptr),
	m_mainAnimation(nullptr),
	m_postAnimation(nullptr),
	m_dt(0.0f),
	m_preAnimationTime(0.0f),
	m_mainAnimationTime(0.0f),
	m_postAnimationTime(0.0f)
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
	
	m_postAnimationState = new AnimationState(m_animationStateData);
	m_postAnimationState->setLooping(true);
	
	m_preAnimationState = new AnimationState(m_animationStateData);
	m_preAnimationState->setLooping(true);
}

string Humanoid::getBodyPartName(const BodyPart part)
{
	switch(part)
	{
	case HEAD:				return "head";
	case NECK:				return "neck";
	case TORSO:				return "torso";
	case HIPS:				return "hips";
	case THIGH_LEFT:		return "lthigh";
	case THIGH_RIGHT:		return "rthigh";
	case SHOULDER_RIGHT:	return "rshoulder";
	case SHOULDER_LEFT:		return "lshoulder";
	case ARM_RIGHT:			return "rarm";
	case ARM_LEFT:			return "larm";
	case LEG_RIGHT:			return "rleg";
	case LEG_LEFT:			return "lleg";
	default:				return "null";
	}
}

Animation *Humanoid::getAnimation(const Anim anim)
{
	string animName = "default";
	switch(anim)
	{
	case ANIM_NULL: return nullptr;
	case ANIM_DEFAULT: animName = "default"; break;
	case ANIM_IDLE: animName = "idle"; break;
	case ANIM_JUMP: animName = "jump"; break;
	case ANIM_WALK: animName = "walk"; break;
	case ANIM_WALL_SLIDE: animName = "wall-slide"; break;
	case ANIM_MINE: animName = "mine"; break;
	}
	return m_skeleton->findAnimation(animName);
}

// ANIMATIONS
void Humanoid::setPreAnimation(const Anim anim)
{
	// Set item animation
	Animation *animations = getAnimation(anim);
	if(m_preAnimation != animations)
	{
		if(animations != nullptr)
		{
			m_preAnimationState->setAnimation(animations);
		}
		m_preAnimation = animations;
	}
}

void Humanoid::setMainAnimation(const Anim anim)
{
	// Set main animation
	Animation *animations = getAnimation(anim);
	if(m_mainAnimation != animations)
	{
		if(animations != nullptr)
		{
			m_mainAnimationState->setAnimation(animations);
		}
		m_mainAnimation = animations;
	}
}

void Humanoid::setPostAnimation(const Anim anim)
{
	// Set item animation
	Animation *animations = getAnimation(anim);
	if(m_postAnimation != animations)
	{
		if(animations != nullptr)
		{
			m_postAnimationState->setAnimation(animations);
		}
		m_postAnimation = animations;
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

void Humanoid::update(const float dt)
{
	m_dt = dt;
	// Update all animations
	if(m_preAnimation)
	{
		m_preAnimationTime = 0.0f;
	}

	if(m_mainAnimation)
	{
		m_mainAnimationTime = 0.0f;
	}

	if(m_postAnimation)
	{
		m_postAnimationTime = 0.0f;
	}
}

void Humanoid::draw(PhysicsBody *body, SpriteBatch *spriteBatch, const float alpha)
{
	// Update all animations
	if(m_preAnimation)
	{
		float next = m_dt * alpha;
		m_preAnimationState->update(next - m_preAnimationTime);
		m_preAnimationTime = next;
	}

	if(m_mainAnimation)
	{
		float next = m_dt * alpha;
		m_mainAnimationState->update(next - m_mainAnimationTime);
		m_mainAnimationTime = next;
	}

	if(m_postAnimation)
	{
		m_postAnimationState->update(m_postAnimationTime * m_dt * (1.0f-alpha));
	}

	// Draw skeleton
	m_skeleton->setPosition(body->getDrawPosition(alpha) + Vector2(body->getSize().x*0.5f, 48.0f));
	GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();
	gfxContext.setProjectionMatrix(spriteBatch->getState().projectionMatix);
	m_skeleton->draw(gfxContext);
	gfxContext.setProjectionMatrix(Matrix4());
}

void Humanoid::drawRightHandSprite(Sprite &sprite, const Vector2 &origin, SpriteBatch *spriteBatch)
{
	float angle = m_skeleton->findBone("rhand")->getWorldRotation() + 180.0f;
	if(m_skeleton->getFlipX())
	{
		sprite.setScaleX(-1.0f);
	}
	else
	{
		sprite.setScaleX(1.0f);
		angle *= -1;
	}
	sprite.setPosition(m_skeleton->getPosition() + m_skeleton->findBone("rhand")->getWorldPosition() - origin);
	sprite.setOrigin(origin);
	sprite.setRotation(angle);
	spriteBatch->drawSprite(sprite);
}