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
	m_animationStateData->setMix("Idle", "Walk", 0.2f);
	m_animationStateData->setMix("Walk", "Idle", 0.5f);
	m_animationStateData->setMix("Jump", "Idle", 0.1f);
	m_animationStateData->setMix("Walk", "Jump", 0.1f);
	m_animationStateData->setMix("Jump", "Idle", 0.1f);
	m_animationStateData->setMix("Idle", "Jump", 0.2f);
	
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

	// Create slots map
	for(int i = 0; i < SLOT_COUNT; i++)
	{
		HumanoidSlot slot = (HumanoidSlot) i;
		m_slots[slot] = m_skeleton->findSlot(getSlotName(slot));
	}

	// Create animation map
	for(int i = 0; i < ANIM_COUNT; i++)
	{
		HumanoidAnim anim = (HumanoidAnim) i;
		m_animations[anim] = m_skeleton->findAnimation(getAnimName(anim));
	}

	// Set render array to false
	for(uint i = 0; i < SLOT_COUNT; ++i)
	{
		m_renderPart[i] = true;
	}

	// Set default appearance
	for(uint i = 0; i < SLOT_COUNT; ++i)
	{
		HumanoidSlot slot = (HumanoidSlot) i;
		m_appearanceName[slot] = getSlotName(slot) + "_Default";
		m_appearanceColor[slot] = 0;

		m_apparelName[slot] = "Default_" + getSlotName(slot);
	}

	m_hairColor = Color(220, 160, 85, 255);
	m_skinColor = Color(255, 210, 155, 255);
	m_eyeColor = Color(220, 160, 85, 255);
	m_lipColor = Color(215, 170, 135, 255);

	m_appearanceColor[HEAD] = &m_skinColor;
	m_appearanceColor[LEFT_LEG] = &m_skinColor;
	m_appearanceColor[RIGHT_LEG] = &m_skinColor;
	m_appearanceColor[LEFT_ARM] = &m_skinColor;
	m_appearanceColor[RIGHT_ARM] = &m_skinColor;
	m_appearanceColor[TORSO] = &m_skinColor;
	m_appearanceColor[HAIR] = &m_hairColor;

	m_appearanceAtlas = new SpineAtlas("Sprites/Characters/Images/Appearance/Appearance.atlas");
	m_apparelAtlas = new SpineAtlas("Sprites/Characters/Images/Apparel/Apparel.atlas");
	m_equipmentAttachmentLoader = new AtlasAttachmentLoader("Sprites/Characters/Images/Equipment/Equipment.atlas");

	m_colorMaskShader = Game::GetInstance()->getResourceManager()->get<Shader>("Shaders/Color_Mask");
}

Humanoid::~Humanoid()
{
	delete m_skeletonRenderTarget;
}

string Humanoid::getSlotName(const HumanoidSlot slot)
{
	switch(slot)
	{
		case HIPS: return "Hips";
		case LEFT_THIGH: return "Left_Thigh";
		case RIGHT_THIGH: return "Right_Thigh";
		case LEFT_LEG: return "Left_Leg";
		case RIGHT_LEG: return "Right_Leg";
		case TORSO: return "Torso";
		case MOUTH: return "Mouth";
		case EYES: return "Eyes";
		case HAIR: return "Hair";
		case HEAD: return "Head";
		case LEFT_SHOULDER: return "Left_Shoulder";
		case RIGHT_SHOULDER: return "Right_Shoulder";
		case LEFT_ARM: return "Left_Arm";
		case RIGHT_ARM: return "Right_Arm";
		case LEFT_HAND: return "Left_Hand";
		case RIGHT_HAND: return "Right_Hand";
	}
	return "";
}

string Humanoid::getAnimName(const HumanoidAnim slot)
{
	switch(slot)
	{
		case ANIM_NULL: return "";
		case ANIM_DEFAULT: return "Default";
		case ANIM_IDLE: return "Idle";
		case ANIM_JUMP: return "Jump";
		case ANIM_WALK: return "Walk";
		case ANIM_WALL_SLIDE: return "Wall_Slide";
		case ANIM_MINE: return "Mine";
		case ANIM_ARROW_AIM_UP: return "Bow_Aim_Up";
		case ANIM_ARROW_AIM_FW: return "Bow_Aim_Forward";
		case ANIM_ARROW_AIM_DW: return "Bow_Aim_Down";
		case ANIM_ROLL_FORWARD: return "Roll_Forward";
		case ANIM_SWORD_ATTACH_LIGHT: return "Sword_Attack_Light";
		case ANIM_HOLD_TORCH: return "Hold_Torch";
	}
	return "";
}

// ANIMATIONS
void Humanoid::setPreAnimation(const HumanoidAnim anim)
{
	// Set pre-animation
	Animation *animations = m_animations[anim];
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

void Humanoid::setMainAnimation(const HumanoidAnim anim)
{
	// Set main animation
	Animation *animations = m_animations[anim];
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

void Humanoid::setPostAnimation(const HumanoidAnim anim)
{
	// Set post animation
	Animation *animation = m_animations[anim];
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

void Humanoid::setPostBlendAnimations(const HumanoidAnim anim1, const HumanoidAnim anim2, const float alpha)
{
	Animation *animation1 = m_animations[anim1];
	Animation *animation2 = m_animations[anim2];
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

	for(uint i = 0; i < SLOT_COUNT; ++i)
	{
		// Do we want to draw over this slot?
		if(m_renderPart[i])
		{
			GraphicsContext *context = spriteBatch->getGraphicsContext();

			// Get skeleton and apparel atlas region
			SpineAtlasRegion *skeletonAtlasRegion = m_skeleton->getAtlas()->findRegion(getSlotName((HumanoidSlot) i));
			SpineAtlasRegion *appearanceAtlasRegion = m_appearanceAtlas->findRegion(m_appearanceName[i].c_str());
			if(skeletonAtlasRegion && appearanceAtlasRegion)
			{
				// Calculate pixel coordinates for the slot we want to draw over
				Resource<Texture2D> skeletonAtlas = m_skeleton->getAtlas()->getTexture();
				TextureRegion skeletonTextureRegion = skeletonAtlasRegion->getTextureRegion();
				uint x0 = skeletonTextureRegion.uv0.x * skeletonAtlas->getWidth(), y0 = skeletonTextureRegion.uv0.y * skeletonAtlas->getHeight(),
					x1 = skeletonTextureRegion.uv1.x * skeletonAtlas->getWidth(), y1 = skeletonTextureRegion.uv1.y * skeletonAtlas->getHeight();

				// Draw the appearance over the slot in the skeleton render target
				context->setRenderTarget(m_skeletonRenderTarget);
				context->disable(GraphicsContext::BLEND);
				if(i == EYES)
				{
					// Eyes needs to be drawn using a special color mask shader
					Vector4F colors[3];
					colors[0] = Vector4F(m_hairColor.getR(), m_hairColor.getG(), m_hairColor.getB(), m_hairColor.getA()) / 255.0f;
					colors[1] = Vector4F(255, 255, 255, 255) / 255.0f;
					colors[2] = Vector4F(m_eyeColor.getR(), m_eyeColor.getG(), m_eyeColor.getB(), m_eyeColor.getA()) / 255.0f;
					m_colorMaskShader->setUniform4f("u_Colors", (float*)colors);
					m_colorMaskShader->setSampler2D("u_Texture", m_appearanceAtlas->getTexture());

					context->setShader(m_colorMaskShader);
					context->drawRectangle(x0, y0, x1 - x0, y1 - y0, Color(255), appearanceAtlasRegion->getTextureRegion());
					context->setShader(0);
				}
				else if(i == MOUTH)
				{
					// Mouth needs to be drawn using a special color mask shader
					Vector4F colors[3];
					colors[0] = Vector4F(m_lipColor.getR(), m_lipColor.getG(), m_lipColor.getB(), m_lipColor.getA()) / 255.0f;
					colors[1] = Vector4F(255, 255, 255, 255) / 255.0f;
					colors[2] = Vector4F(255, 255, 255, 255) / 255.0f;
					m_colorMaskShader->setUniform4f("u_Colors", (float*) colors);
					m_colorMaskShader->setSampler2D("u_Texture", m_appearanceAtlas->getTexture());

					context->setShader(m_colorMaskShader);
					context->drawRectangle(x0, y0, x1 - x0, y1 - y0, Color(255), appearanceAtlasRegion->getTextureRegion());
					context->setShader(0);
				}
				else
				{
					context->setTexture(m_appearanceAtlas->getTexture());
					context->drawRectangle(x0, y0, x1 - x0, y1 - y0, m_appearanceColor[i] ? *m_appearanceColor[i] : Color(255), appearanceAtlasRegion->getTextureRegion());
				}
				context->enable(GraphicsContext::BLEND);

				SpineAtlasRegion *apparelAtlasRegion = m_apparelAtlas->findRegion(m_apparelName[i].c_str());
				if(apparelAtlasRegion)
				{
					// Draw the apparel over the slot in the skeleton render target
					context->setTexture(m_apparelAtlas->getTexture());
					context->drawRectangle(x0, y0, x1 - x0, y1 - y0, Color(255), apparelAtlasRegion->getTextureRegion());
				}
				context->setTexture(0);
				context->setRenderTarget(0);
			}

			// Don't render again
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

bool Humanoid::setAppearance(const HumanoidSlot slot, const string &appearanceName)
{
	// Set appearance image name
	const string name = getSlotName(slot) + "_" + appearanceName;
	SpineAtlasRegion *atlasRegion = m_appearanceAtlas->findRegion(name);
	if(atlasRegion)
	{
		m_appearanceName[slot] = name;
		m_renderPart[slot] = true;
		return true;
	}
	
	LOG("Appearance image with name '%s' not found", name.c_str());
	return false;
}

RegionAttachment *Humanoid::setAttachment(const HumanoidSlot slot, const string &name, const string &path)
{
	RegionAttachment *attachment = m_equipmentAttachmentLoader->newAttachment(name.c_str(), path.c_str());
	if(attachment)
	{
		m_slots[slot]->setAttachment(attachment);
	}
	return attachment;
}

void Humanoid::clearAttachment(const HumanoidSlot slot)
{
	m_slots[slot]->setAttachment(0);
}

bool Humanoid::setApparel(const HumanoidSlot slot, const string &apparelName)
{
	// Set appearance image name
	const string name = apparelName + "_" + getSlotName(slot);
	SpineAtlasRegion *atlasRegion = m_apparelAtlas->findRegion(name); // TODO: Optimization: can store the atlasRegion itself instead of a string for the name
	if(atlasRegion)
	{
		m_apparelName[slot] = name;
		m_renderPart[slot] = true;
		return true;
	}

	LOG("Apparel image with name '%s' not found", name.c_str());
	return false;
}
