#include "Humanoid.h"
#include "Constants.h"

#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Animation/Slot.h"
#include "Animation/RegionAttachment.h"
#include "Animation/AtlasAttachmentLoader.h"
#include "Animation/Atlas.h"

#include "DynamicEntity.h"

Humanoid::Humanoid() :
	m_preAnimation(nullptr),
	m_mainAnimation(nullptr),
	m_postAnimation(nullptr),
	m_postAnimationMix(nullptr),
	m_prevPreAnimationTime(0.0f),
	m_preAnimationTime(0.0f),
	m_prevMainAnimationTime(0.0f),
	m_mainAnimationTime(0.0f),
	m_prevPostAnimationTime(0.0f),
	m_postAnimationTime(0.0f),
	m_postAnimMixAlpha(0.0f)
{
	// Load skeleton data
	m_skeleton = new Skeleton("Sprites/Characters/Skeleton.json", "Sprites/Characters/Skeleton.atlas", 1.0f);
	m_skeleton->getAtlas()->getTexture()->setFiltering(Texture2D::LINEAR);

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

	// Create render target
	m_skeletonRenderTarget = new RenderTarget2D(m_skeleton->getAtlas()->getTexture());

	// Set render array to false
	for(uint i = 0; i < BODY_PART_COUNT; ++i)
	{
		m_renderPart[i] = false;
	}

	// Set default appearance
	m_appearance[HEAD] = "Default_Head";
	m_appearance[ARM_LEFT] = "Default_Left_Arm";
	m_appearance[ARM_RIGHT] = "Default_Right_Arm";
	m_appearance[TORSO] = "Default_Torso";
	m_appearance[THIGH_LEFT] = "Default_Left_Thigh";
	m_appearance[THIGH_RIGHT] = "Default_Right_Thigh";
	m_appearance[SHOULDER_LEFT] = "Default_Left_Shoulder";
	m_appearance[SHOULDER_RIGHT] = "Default_Right_Shoulder";
	m_appearance[HIPS] = "Default_Hips";
	m_appearance[LEG_LEFT] = "Default_Left_Leg";
	m_appearance[LEG_RIGHT] = "Default_Right_Leg";
	m_appearance[HAND_RIGHT] = "Head_Less"; // TODO: Should not be here
	m_appearance[HAND_LEFT] = "Head_Less";

	m_appearanceAtlas = new SpineAtlas("Sprites/Characters/Images/Apparel/Apparel.atlas");
	m_equipmentAttachmentLoader = new AtlasAttachmentLoader("Sprites/Characters/Images/Equipment/Equipment.atlas");
}

Humanoid::~Humanoid()
{
	delete m_skeletonRenderTarget;
}

string Humanoid::getBodyPartName(const BodyPart part)
{
	switch(part)
	{
	case HEAD:				return "Head";
	case TORSO:				return "Torso";
	case HIPS:				return "Hips";
	case THIGH_LEFT:		return "Left_Thigh";
	case THIGH_RIGHT:		return "Right_Thigh";
	case SHOULDER_RIGHT:	return "Right_Shoulder";
	case SHOULDER_LEFT:		return "Left_Shoulder";
	case ARM_RIGHT:			return "Right_Arm";
	case ARM_LEFT:			return "Left_Arm";
	case LEG_RIGHT:			return "Right_Leg";
	case LEG_LEFT:			return "Left_Leg";
	case HAND_RIGHT:		return "Right_Hand";
	case HAND_LEFT:			return "Left_Hand";
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
	case ANIM_ARROW_AIM_UP: animName = "arrow_aim_up"; break;
	case ANIM_ARROW_AIM_FW: animName = "arrow_aim_fw"; break;
	case ANIM_ARROW_AIM_DW: animName = "arrow_aim_dw"; break;
	case ANIM_HOLD_TORCH: animName = "Hold_Torch"; break;
	}
	return m_skeleton->findAnimation(animName);
}

// ANIMATIONS
void Humanoid::setPreAnimation(const Anim anim)
{
	// Set pre animation
	Animation *animations = getAnimation(anim);
	if(m_preAnimation != animations)
	{
		if(animations != nullptr)
		{
			m_preAnimationState->setAnimation(animations);
		}
		m_preAnimation = animations;
		m_prevPreAnimationTime = m_preAnimationTime = 0.0f;
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
		m_prevPreAnimationTime = m_preAnimationTime = 0.0f;
	}
}

void Humanoid::setPostAnimation(const Anim anim)
{
	// Set post animation
	Animation *animation = getAnimation(anim);
	if(m_postAnimation != animation)
	{
		if(animation != nullptr)
		{
			m_postAnimationState->setAnimation(animation);
		}
		m_postAnimation = animation;
		m_prevPostAnimationTime = m_postAnimationTime = 0.0f;
		m_postAnimationMix = nullptr;
	}
}

void Humanoid::setPostBlendAnimations(const Anim anim1, const Anim anim2, const float alpha)
{
	Animation *animation1 = getAnimation(anim1);
	Animation *animation2 = getAnimation(anim2);
	if(m_postAnimation != animation1 || m_postAnimationMix != animation2)
	{
		if(m_postAnimation != animation1 && m_postAnimationMix != animation2)
			m_prevPostAnimationTime = m_postAnimationTime = 0.0f;
		m_postAnimation = animation1;
		m_postAnimationMix = animation2;
	}
	m_postAnimMixAlpha = alpha;
}

void Humanoid::onTick(TickEvent *e)
{
	// Update all animations
	if(m_preAnimation)
	{
		m_prevPreAnimationTime = m_preAnimationTime;
		m_preAnimationTime += e->getDelta();
	}

	if(m_mainAnimation)
	{
		m_prevMainAnimationTime = m_mainAnimationTime;
		m_mainAnimationTime += e->getDelta();
	}

	if(m_postAnimation)
	{
		m_prevPostAnimationTime = m_postAnimationTime;
		m_postAnimationTime += e->getDelta();
	}
}

void Humanoid::draw(DynamicEntity *body, SpriteBatch *spriteBatch, const float alpha)
{
	// Update all animations
	if(m_preAnimation)
	{
		m_preAnimationState->setTime(math::lerp(m_prevPreAnimationTime, m_preAnimationTime, alpha));
	}

	if(m_mainAnimation)
	{
		m_mainAnimationState->setTime(math::lerp(m_prevMainAnimationTime, m_mainAnimationTime, alpha));
	}

	if(m_postAnimation)
	{
		if(m_postAnimationMix)
		{
			m_postAnimation->setTime(math::lerp(m_prevPostAnimationTime, m_postAnimationTime, alpha));
			m_postAnimationMix->setTimeAndMix(math::lerp(m_prevPostAnimationTime, m_postAnimationTime, alpha), m_postAnimMixAlpha);
		}
		else
		{
			m_postAnimationState->setTime(math::lerp(m_prevMainAnimationTime, m_mainAnimationTime, alpha));
		}
	}

	for(uint i = 0; i < BODY_PART_COUNT; ++i)
	{
		// Do we need to re-render body part?
		if(m_renderPart[i])
		{
			GraphicsContext *context = spriteBatch->getGraphicsContext();
			Resource<Texture2D> skeletonAtlas = m_skeleton->getAtlas()->getTexture();

			context->setRenderTarget(m_skeletonRenderTarget);

			// Get body part region
			TextureRegion region = m_skeleton->getAtlas()->findRegion(getBodyPartName((BodyPart) i))->getTextureRegion();
			uint x0 = region.uv0.x * skeletonAtlas->getWidth(), y0 = region.uv0.y * skeletonAtlas->getHeight(),
				x1 = region.uv1.x * skeletonAtlas->getWidth(), y1 = region.uv1.y * skeletonAtlas->getHeight();

			// Draw apparel to the region
			context->disable(GraphicsContext::BLEND);

			SpineAtlasRegion *tt = m_appearanceAtlas->findRegion(m_appearance[i].c_str());

			context->setTexture(m_appearanceAtlas->getTexture());
			context->drawRectangle(x0, y0, x1 - x0, y1 - y0, Color(255), tt->getTextureRegion());

			// Reset context
			context->enable(GraphicsContext::BLEND);
			context->setTexture(0);
			context->setRenderTarget(0);

			// Don't need to render again
			m_renderPart[i] = false;
		}
	}

	// Draw skeleton
	m_skeleton->setPosition(body->getDrawPosition(alpha) + Vector2F(body->getSize().x * 0.5f, 48.0f));
	GraphicsContext *gfxContext = spriteBatch->getGraphicsContext();
	gfxContext->setTransformationMatrix(spriteBatch->getState().transformationMatix);
	m_skeleton->draw(gfxContext);
	gfxContext->setTransformationMatrix(Matrix4());
}

void Humanoid::setAppearance(const BodyPart part, const string &name)
{
	// Set appearance image name
	SpineAtlasRegion *atlasRegion = m_appearanceAtlas->findRegion(name);
	if(atlasRegion)
	{
		TextureRegion bodyPartRegion = m_skeleton->getAtlas()->findRegion(getBodyPartName(part))->getTextureRegion();
		Resource<Texture2D> skeletonAtlasTexture = m_skeleton->getAtlas()->getTexture();
		uint bodyPartWidth = (bodyPartRegion.uv1.x - bodyPartRegion.uv0.x) * skeletonAtlasTexture->getWidth();
		uint bodyPartHeight = (bodyPartRegion.uv1.y - bodyPartRegion.uv0.y) * skeletonAtlasTexture->getHeight();

		TextureRegion appearanceRegion = atlasRegion->getTextureRegion();
		Resource<Texture2D> appearanceAtlasTexture = m_appearanceAtlas->getTexture();
		uint appearanceWidth = (appearanceRegion.uv1.x - appearanceRegion.uv0.x) * appearanceAtlasTexture->getWidth();
		uint appearanceHeight = (appearanceRegion.uv1.y - appearanceRegion.uv0.y) * appearanceAtlasTexture->getHeight();

		// TODO: (Maybe) Make a system where this is unnecessary (for instance, have all of the same body part in one atlas)
		if(appearanceWidth == bodyPartWidth && appearanceHeight == bodyPartHeight)
		{
			m_appearance[part] = name;
			m_renderPart[part] = true;
		}
		else
		{
			LOG("Appearance image and body part region size needs to match");
		}
	}
	else
	{
		LOG("Appearance image with name '%s' not found", name.c_str());
	}
}

RegionAttachment *Humanoid::setAttachment(const BodyPart part, const string &name, const string &path)
{
	RegionAttachment *attachment = m_equipmentAttachmentLoader->newAttachment(name.c_str(), path.c_str());
	if(attachment)
	{
		Slot *slot = m_skeleton->findSlot(getBodyPartName(part));
		if(slot)
		{
			slot->setAttachment(attachment);
		}
	}
	return attachment;
}

void Humanoid::clearAttachment(const BodyPart part)
{
	Slot* slot = m_skeleton->findSlot(getBodyPartName(part));
	if(slot)
	{
		slot->setAttachment(0);
	}
}