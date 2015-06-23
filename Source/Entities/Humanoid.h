#ifndef HUMANOID_H
#define HUMANOID_H

#include "Config.h"

class Skeleton;
class AnimationStateData;
class AnimationState;
class Animation;
class DynamicEntity;

class Humanoid
{
public:
	Humanoid();

	enum BodyPart
	{
		HEAD,
		ARM_LEFT,
		ARM_RIGHT,
		TORSO,
		THIGH_LEFT,
		THIGH_RIGHT,
		SHOULDER_LEFT,
		SHOULDER_RIGHT,
		NECK,
		HIPS,
		LEG_LEFT,
		LEG_RIGHT
	};

	enum Anim
	{
		ANIM_NULL,
		ANIM_DEFAULT,
		ANIM_IDLE,
		ANIM_JUMP,
		ANIM_WALK,
		ANIM_WALL_SLIDE,
		ANIM_MINE
	};
	
	void setPreAnimation(const Anim anim);
	AnimationState *getPreAnimationState() const { return m_preAnimationState; }
	void setMainAnimation(const Anim anim);
	AnimationState *getMainAnimationState() const { return m_mainAnimationState; }
	void setPostAnimation(const Anim anim);
	AnimationState *getPostAnimationState() const { return m_postAnimationState; }
	void setBodyPart(const BodyPart part, const Pixmap &pixmap);

	Skeleton *getSkeleton() { return m_skeleton; }

	void update(const float delta);
	void draw(DynamicEntity *body, SpriteBatch *spriteBatch, const float alpha);
	void drawLeftHandSprite(Sprite &sprite, const Vector2 &origin, SpriteBatch *spriteBatch);
	void drawRightHandSprite(Sprite &sprite, const Vector2 &origin, SpriteBatch *spriteBatch);

private:
	string getBodyPartName(const BodyPart part);
	Animation *getAnimation(const Anim anim);

	// Skeletal animations
	Skeleton *m_skeleton;
	AnimationStateData *m_animationStateData;
	AnimationState *m_preAnimationState;
	Animation *m_preAnimation;
	AnimationState *m_mainAnimationState;
	Animation *m_mainAnimation;
	AnimationState *m_postAnimationState;
	Animation *m_postAnimation;

	float m_prevPreAnimationTime;
	float m_preAnimationTime;
	float m_prevMainAnimationTime;
	float m_mainAnimationTime;
	float m_prevPostAnimationTime;
	float m_postAnimationTime;
};

#endif // HUMANOID_H