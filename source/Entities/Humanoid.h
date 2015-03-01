#ifndef HUMANOID_H
#define HUMANOID_H

#include "Config.h"

class Skeleton;
class AnimationStateData;
class AnimationState;
class Animation;
class PhysicsBody;

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

	enum Animation
	{
		DEFAULT,
		IDLE,
		JUMP,
		WALK,
		WALL_SLIDE,
		MINE
	};
	
	void setMainAnimation(const string &name);
	void setItemAnimation(Animation *anim);
	void setBodyPart(const BodyPart part, const Pixmap &pixmap);

	Skeleton *getSkeleton() { return m_skeleton; }

	void update();
	void draw(PhysicsBody *body, SpriteBatch *spriteBatch);
	void drawLeftHandSprite(Sprite &sprite, const Vector2 &origin, SpriteBatch *spriteBatch);
	void drawRightHandSprite(Sprite &sprite, const Vector2 &origin, SpriteBatch *spriteBatch);

private:
	string getBodyPartName(const BodyPart part);

	// Skeletal animations
	Skeleton *m_skeleton;
	AnimationStateData *m_animationStateData;
	AnimationState *m_mainAnimationState;
	Animation *m_currentAnim;
	AnimationState *m_itemAnimationState;
	Animation *m_itemAnimation;
};

#endif // HUMANOID_H