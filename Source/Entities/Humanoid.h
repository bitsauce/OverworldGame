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
class SpineAtlasRegion;
class Slot;

class Humanoid
{
public:
	Humanoid();
	~Humanoid();

	enum BodySlot
	{
		HIPS,
		LEFT_THIGH,
		RIGHT_THIGH,
		LEFT_LEG,
		RIGHT_LEG,
		TORSO,
		HEAD,
		HAIR, // /HEADWEAR
		MOUTH,
		EYES,
		LEFT_SHOULDER,
		RIGHT_SHOULDER,
		LEFT_ARM,
		RIGHT_ARM,
		LEFT_HAND,
		RIGHT_HAND,
		SLOT_COUNT
	};

	enum Anim
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
	
	void setPreAnimation(const Anim anim);
	AnimationState *getPreAnimationState() const { return m_preAnimationState; }
	void setMainAnimation(const Anim anim);
	AnimationState *getMainAnimationState() const { return m_mainAnimationState; }
	void setPostAnimation(const Anim anim);
	void setPostBlendAnimations(const Anim anim1, const Anim anim2, const float alpha);
	AnimationState *getPostAnimationState() const { return m_postAnimationState; }

	Skeleton *getSkeleton() { return m_skeleton; }

	void onTick(TickEvent *e);
	void draw(DynamicEntity *body, SpriteBatch *spriteBatch, const float alpha);

	bool setAppearance(const BodySlot slot, const uint layer, const string &name);

	RegionAttachment *setAttachment(const BodySlot slot, const string &newAttachmentName, const string &attachmentPath);
	void clearAttachment(const BodySlot slot);

	void setSkinColor(const Color &color)
	{
		m_skinColor = color;
		m_renderSlot[HEAD] = true;
		m_renderSlot[LEFT_LEG] = true;
		m_renderSlot[RIGHT_LEG] = true;
		m_renderSlot[LEFT_ARM] = true;
		m_renderSlot[RIGHT_ARM] = true;
		m_renderSlot[TORSO] = true;
	}

	void setHairColor(const Color &color)
	{
		m_hairColor = color;
		m_renderSlot[HAIR] = true;
		m_renderSlot[EYES] = true;
	}

	void setEyeColor(const Color &color)
	{
		m_eyeColor = color;
		m_renderSlot[EYES] = true;
	}

	void setLipColor(const Color &color)
	{
		m_lipColor = color;
		m_renderSlot[MOUTH] = true;
	}

private:
	string getSlotName(const BodySlot slot);
	string getAnimName(const Anim anim);

	// Skeleton data
	RenderTarget2D *m_skeletonRenderTarget;
	map<BodySlot, Slot*> m_slots;
	map<Anim, Animation*> m_animations;

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
	bool m_renderSlot[SLOT_COUNT];
	vector<SpineAtlasRegion*> m_appearanceRegion[SLOT_COUNT];
	SpineAtlas *m_appearanceAtlas;

	bool m_overrideHair;

	// Colors
	Color m_skinColor, m_eyeColor, m_hairColor, m_lipColor;

	// Color mask shader
	Resource<Shader> m_colorMaskShader;
};

#endif // HUMANOID_H