#include "Humanoid.h"
#include "Constants.h"

#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Animation/Slot.h"
#include "Animation/RegionAttachment.h"
#include "Animation/AtlasAttachmentLoader.h"
#include "Animation/Atlas.h"

#include "Entity.h"

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
	m_postAnimMixAlpha(0.0f),
	m_overrideHair(false)
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
		BodySlot slot = (BodySlot) i;
		m_slots[slot] = m_skeleton->findSlot(getSlotName(slot));
	}

	// Create animation map
	for(int i = 0; i < ANIM_COUNT; i++)
	{
		Anim anim = (Anim) i;
		m_animations[anim] = m_skeleton->findAnimation(getAnimName(anim));
	}

	m_appearanceAtlas = new SpineAtlas("Sprites/Characters/Images/Appearance/Appearance.atlas");
	m_equipmentAttachmentLoader = new AtlasAttachmentLoader("Sprites/Characters/Images/Equipment/Equipment.atlas");

	m_colorMaskShader = Game::GetInstance()->getResourceManager()->get<Shader>("Shaders/Color_Mask");

	// Set render array to false
	for(uint i = 0; i < SLOT_COUNT; ++i)
	{
		m_renderSlot[i] = true;
	}

	// Set default appearance
	for(uint i = 0; i < SLOT_COUNT; ++i)
	{
		BodySlot slot = (BodySlot) i;
		m_appearanceRegion[slot].push_back(m_appearanceAtlas->findRegion("Body_Default_" + getSlotName(slot)));
		m_appearanceRegion[slot].push_back(m_appearanceAtlas->findRegion("Costume_Default_" + getSlotName(slot)));
	}

	m_hairColor = Color(220, 160, 85, 255);
	m_skinColor = Color(255, 210, 155, 255);
	m_eyeColor = Color(220, 160, 85, 255);
	m_lipColor = Color(215, 170, 135, 255);
}

Humanoid::~Humanoid()
{
	delete m_skeletonRenderTarget;
}

string Humanoid::getSlotName(const BodySlot slot)
{
	switch(slot)
	{
		case HIPS: return "Hips";
		case LEFT_THIGH: return "Left_Thigh";
		case RIGHT_THIGH: return "Right_Thigh";
		case LEFT_LEG: return "Left_Leg";
		case RIGHT_LEG: return "Right_Leg";
		case TORSO: return "Torso";
		case HEAD: return "Head";
		case MOUTH: return "Mouth";
		case EYES: return "Eyes";
		case HAIR: return "Hair";
		case LEFT_SHOULDER: return "Left_Shoulder";
		case RIGHT_SHOULDER: return "Right_Shoulder";
		case LEFT_ARM: return "Left_Arm";
		case RIGHT_ARM: return "Right_Arm";
		case LEFT_HAND: return "Left_Hand";
		case RIGHT_HAND: return "Right_Hand";
	}
	return "";
}

string Humanoid::getAnimName(const Anim slot)
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
void Humanoid::setPreAnimation(const Anim anim)
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

void Humanoid::setMainAnimation(const Anim anim)
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

void Humanoid::setPostAnimation(const Anim anim)
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

void Humanoid::setPostBlendAnimations(const Anim anim1, const Anim anim2, const float alpha)
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

void Humanoid::draw(Entity *body, SpriteBatch *spriteBatch, const float alpha)
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
		if(m_renderSlot[i])
		{
			GraphicsContext *context = spriteBatch->getGraphicsContext();
			context->setRenderTarget(m_skeletonRenderTarget);
			context->setTexture(m_appearanceAtlas->getTexture());

			SpineAtlasRegion *skeletonAtlasRegion = m_skeleton->getAtlas()->findRegion(getSlotName((BodySlot) i));
			if(skeletonAtlasRegion)
			{
				// Calculate pixel coordinates for the slot we want to draw over
				Resource<Texture2D> skeletonAtlas = m_skeleton->getAtlas()->getTexture();
				TextureRegion skeletonTextureRegion = skeletonAtlasRegion->getTextureRegion();
				uint x0 = skeletonTextureRegion.uv0.x * skeletonAtlas->getWidth(), y0 = skeletonTextureRegion.uv0.y * skeletonAtlas->getHeight(),
					x1 = skeletonTextureRegion.uv1.x * skeletonAtlas->getWidth(), y1 = skeletonTextureRegion.uv1.y * skeletonAtlas->getHeight();

				// For apparence + apperal
				context->disable(GraphicsContext::BLEND);
				for(uint j = 0; j < m_appearanceRegion[i].size(); j++)
				{
					// Get appearance atlas region
					SpineAtlasRegion *appearanceAtlasRegion = m_appearanceRegion[i][j];
					if(appearanceAtlasRegion)
					{
						// Draw the appearance over the slot in the skeleton render target
						switch(i)
						{
							case EYES:
							{
								// Eyes needs to be drawn using a special color mask shader
								Vector4F colors[3];
								colors[0] = Vector4F(m_hairColor.getR(), m_hairColor.getG(), m_hairColor.getB(), m_hairColor.getA()) / 255.0f;
								colors[1] = Vector4F(255, 255, 255, 255) / 255.0f;
								colors[2] = Vector4F(m_eyeColor.getR(), m_eyeColor.getG(), m_eyeColor.getB(), m_eyeColor.getA()) / 255.0f;
								m_colorMaskShader->setUniform4f("u_Colors", (float*) colors);
								m_colorMaskShader->setSampler2D("u_Texture", m_appearanceAtlas->getTexture());

								context->setShader(m_colorMaskShader);
								context->drawRectangle(x0, y0, x1 - x0, y1 - y0, Color(255), appearanceAtlasRegion->getTextureRegion());
								context->setShader(0);
							}
							break;

							case MOUTH:
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
							break;

							case HAIR:
							{
								if(j == 0)
								{
									if(m_overrideHair)
									{
										context->drawRectangle(x0, y0, x1 - x0, y1 - y0, Color(0, 0, 0, 0));
									}
									else
									{
										context->drawRectangle(x0, y0, x1 - x0, y1 - y0, m_hairColor, appearanceAtlasRegion->getTextureRegion());
									}
								}
								else
								{
									context->drawRectangle(x0, y0, x1 - x0, y1 - y0, Color(255), appearanceAtlasRegion->getTextureRegion());
								}
							}
							break;

							default:
							{
								context->drawRectangle(x0, y0, x1 - x0, y1 - y0, j == 0 ? m_skinColor : Color(255), appearanceAtlasRegion->getTextureRegion());
							}
							break;
						}
						context->enable(GraphicsContext::BLEND);
					}
				}
			}

			context->setTexture(0);
			context->setRenderTarget(0);

			// Don't render again
			m_renderSlot[i] = false;
		}
	}

	// Draw skeleton
	m_skeleton->setPosition(body->getDrawPosition(alpha) + Vector2F(body->getSize().x * 0.5f, 48.0f));
	GraphicsContext *gfxContext = spriteBatch->getGraphicsContext();
	gfxContext->setTransformationMatrix(spriteBatch->getState().transformationMatix);
	m_skeleton->draw(gfxContext);
	gfxContext->setTransformationMatrix(Matrix4());
}

bool Humanoid::setAppearance(const BodySlot slot, const uint layer, const string &appearanceName)
{
	if(slot == HAIR)
	m_overrideHair = appearanceName != "Default";

	// Set appearance image name
	const string name = string(layer == 0 ? "Body" : "Costume") + "_" + appearanceName + "_" + getSlotName(slot);
	SpineAtlasRegion *atlasRegion = m_appearanceAtlas->findRegion(name);
	if(atlasRegion)
	{
		m_appearanceRegion[slot][layer] = atlasRegion;
		m_renderSlot[slot] = true;
		return true;
	}
	
	LOG("Appearance image with name '%s' not found", name.c_str());
	return false;
}

RegionAttachment *Humanoid::setAttachment(const BodySlot slot, const string &name, const string &path)
{
	RegionAttachment *attachment = m_equipmentAttachmentLoader->newAttachment(name.c_str(), path.c_str());
	if(attachment)
	{
		m_slots[slot]->setAttachment(attachment);
	}
	return attachment;
}

void Humanoid::clearAttachment(const BodySlot slot)
{
	m_slots[slot]->setAttachment(0);
}