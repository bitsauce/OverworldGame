#ifndef HUMANOID_H
#define HUMANOID_H

#include "Config.h"

class Skeleton;
class AnimationStateData;
class AnimationState;
class Animation;
class DynamicEntity;
class AtlasAttachmentLoader;
class RegionAttachment;
class SpineAtlas;
class Slot;

class Humanoid
{
public:
	Humanoid();
	~Humanoid();

	enum HumanoidSlot
	{
		HIPS,
		LEFT_THIGH,
		RIGHT_THIGH,
		LEFT_LEG,
		RIGHT_LEG,
		TORSO,
		EYES,
		HAIR,
		HEAD,
		LEFT_SHOULDER,
		RIGHT_SHOULDER,
		LEFT_ARM,
		RIGHT_ARM,
		LEFT_HAND,
		RIGHT_HAND,
		SLOT_COUNT
	};

	enum HumanoidAnim
	{
		ANIM_NULL,
		ANIM_DEFAULT,
		ANIM_IDLE,
		ANIM_JUMP,
		ANIM_WALK,
		ANIM_WALL_SLIDE,
		ANIM_MINE,
		ANIM_ARROW_AIM_UP,
		ANIM_ARROW_AIM_FW,
		ANIM_ARROW_AIM_DW,
		ANIM_ROLL_FORWARD,
		ANIM_SWORD_ATTACH_LIGHT,
		ANIM_HOLD_TORCH,
		ANIM_COUNT
	};
	
	void setPreAnimation(const HumanoidAnim anim);
	AnimationState *getPreAnimationState() const { return m_preAnimationState; }
	void setMainAnimation(const HumanoidAnim anim);
	AnimationState *getMainAnimationState() const { return m_mainAnimationState; }
	void setPostAnimation(const HumanoidAnim anim);
	void setPostBlendAnimations(const HumanoidAnim anim1, const HumanoidAnim anim2, const float alpha);
	AnimationState *getPostAnimationState() const { return m_postAnimationState; }

	Skeleton *getSkeleton() { return m_skeleton; }

	void onTick(TickEvent *e);
	void draw(DynamicEntity *body, SpriteBatch *spriteBatch, const float alpha);

	void setAppearance(const HumanoidSlot slot, const string &name);
	RegionAttachment *setAttachment(const HumanoidSlot slot, const string &newAttachmentName, const string &attachmentPath);
	void clearAttachment(const HumanoidSlot slot);

private:
	string getSlotName(const HumanoidSlot slot);
	string getAnimName(const HumanoidAnim anim);

	// Skeleton data
	RenderTarget2D *m_skeletonRenderTarget;
	map<HumanoidSlot, Slot*> m_slots;
	map<HumanoidAnim, Animation*> m_animations;

	// Skeletal animations
	Skeleton *m_skeleton;
	AnimationStateData *m_animationStateData;
	AnimationState *m_preAnimationState;
	Animation *m_preAnimation;
	AnimationState *m_mainAnimationState;
	Animation *m_mainAnimation;

	AnimationState *m_postAnimationState;
	Animation *m_postAnimation;
	Animation *m_postAnimationMix;
	float m_postAnimMixAlpha;

	float m_prevPreAnimationTime;
	float m_preAnimationTime;
	float m_prevMainAnimationTime;
	float m_mainAnimationTime;
	float m_prevPostAnimationTime;
	float m_postAnimationTime;

	// Equipment loading
	AtlasAttachmentLoader *m_equipmentAttachmentLoader;

	// Appearance rendering
	bool m_renderPart[SLOT_COUNT];
	string m_appearance[SLOT_COUNT];
	SpineAtlas *m_appearanceAtlas;
};

#endif // HUMANOID_H