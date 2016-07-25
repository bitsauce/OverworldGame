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
		MOUTH,
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

	bool setAppearance(const HumanoidSlot slot, const string &name);
	RegionAttachment *setAttachment(const HumanoidSlot slot, const string &newAttachmentName, const string &attachmentPath);
	void clearAttachment(const HumanoidSlot slot);

	bool setApparel(const HumanoidSlot slot, const string &name);

	void setSkinColor(const Color &color)
	{
		m_skinColor = color;
		m_renderPart[HEAD] = true;
		m_renderPart[LEFT_LEG] = true;
		m_renderPart[RIGHT_LEG] = true;
		m_renderPart[LEFT_ARM] = true;
		m_renderPart[RIGHT_ARM] = true;
		m_renderPart[TORSO] = true;
	}

	void setHairColor(const Color &color)
	{
		m_hairColor = color;
		m_renderPart[HAIR] = true;
		m_renderPart[EYES] = true;
	}

	void setEyeColor(const Color &color)
	{
		m_eyeColor = color;
		m_renderPart[EYES] = true;
	}

	void setLipColor(const Color &color)
	{
		m_lipColor = color;
		m_renderPart[MOUTH] = true;
	}

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
	string m_appearanceName[SLOT_COUNT];
	Color *m_appearanceColor[SLOT_COUNT];
	SpineAtlas *m_appearanceAtlas;

	string m_apparelName[SLOT_COUNT];
	//Color *m_apparelColor[SLOT_COUNT];
	SpineAtlas *m_apparelAtlas;

	// Colors...
	Color m_skinColor, m_eyeColor, m_hairColor, m_lipColor;

	Resource<Shader> m_colorMaskShader;
};

#endif // HUMANOID_H