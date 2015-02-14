#include "Player.h"
#include "Game/World.h"
#include "Game/Camera.h"
#include "Terrain/Terrain.h"
#include "Constants.h"
#include "Physics/PhysicsBody.h"
#include "Game.h"

#include "Animation/Animation.h"
#include "Animation/Skeleton.h"

Player::Player() :
	GameObject(DRAW_ORDER_PLAYER),
	m_camera(World::getCamera()),
	m_terrain(World::getTerrain()),
	m_jumpTimer(1.0f),
	m_canJump(false),
	m_currentAnim(nullptr),
	m_selectedItemSlot(0),
	m_itemContainer(10)
{
	// Load physics
	m_body = new PhysicsBody();
	m_body->setSize(24, 48);

	// Load skeleton data
	m_skeleton = new Skeleton(":/sprites/characters/anim/skeleton.json", ":/sprites/characters/anim/skeleton.atlas", 1.0f);
	m_skeleton->getTexture()->setFiltering(Texture2D::LINEAR);

	// Setup spine animations
	m_animationStateData = new AnimationStateData(m_skeleton);
	m_animationStateData->setMix("idle", "walk", 0.2f);
	m_animationStateData->setMix("walk", "idle", 0.5f);
	m_animationStateData->setMix("jump", "idle", 0.1f);
	m_animationStateData->setMix("walk", "jump", 0.1f);
	m_animationStateData->setMix("jump", "idle", 0.1f);
	m_animationStateData->setMix("idle", "jump", 0.2f);
	
	// Create spine animation states
	m_walkAnimationState = new AnimationState(m_animationStateData);
	//m_walkAnimationState->setEventCallback(animationEvent);
	m_walkAnimationState->setLooping(true);
	
	m_itemAnimationState = new AnimationState(m_animationStateData);
	m_itemAnimationState->setLooping(true);

	m_skeleton->setFlipY(true); // TODO: Investigate why this is needed
	changeAnimation("idle");

	// Bind keys to item slots
	Input::bind(XD_KEY_1, bind(&Player::setSelectedItemSlot, this, 0));
	Input::bind(XD_KEY_2, bind(&Player::setSelectedItemSlot, this, 1));
	Input::bind(XD_KEY_3, bind(&Player::setSelectedItemSlot, this, 2));
	Input::bind(XD_KEY_4, bind(&Player::setSelectedItemSlot, this, 3));
	Input::bind(XD_KEY_5, bind(&Player::setSelectedItemSlot, this, 4));
	Input::bind(XD_KEY_6, bind(&Player::setSelectedItemSlot, this, 5));
	Input::bind(XD_KEY_7, bind(&Player::setSelectedItemSlot, this, 6));
	Input::bind(XD_KEY_8, bind(&Player::setSelectedItemSlot, this, 7));
	Input::bind(XD_KEY_9, bind(&Player::setSelectedItemSlot, this, 8));
	Input::bind(XD_KEY_0, bind(&Player::setSelectedItemSlot, this, 9));

	World::s_players.push_back(this);
}

// ANIMATIONS
void Player::changeAnimation(const string &name)
{
	// Get animation by name
	Animation *anim = m_skeleton->findAnimation(name);
	if(anim == nullptr)
	{
		LOG("Humanoid::changeAnimation() - Animation '%s' does not exists.", name);
		return;
	}
		
	// Make sure this animation isn't current
	if(m_currentAnim != anim)
	{
		m_walkAnimationState->setAnimation(anim);
		m_currentAnim = anim;
	}
}

void Player::update()
{
	// Jumping
	if(m_body->isContact(SOUTH))
	{
		if(Input::getKeyState(XD_KEY_SPACE))
		{
			if(m_canJump)
			{
				m_body->applyImpulse(Vector2(0.0f, -4.5f));
				m_jumpTimer = 0.0f;
				m_canJump = false;
			}
		}
		else
		{
			m_canJump = true;
		}
	}
	else
	{
		if(m_jumpTimer < 0.1f)
		{
			if(Input::getKeyState(XD_KEY_SPACE)) // High/low jumping
			{
				m_body->applyImpulse(Vector2(0.0f, -0.75f));
			}
			m_jumpTimer += Graphics::getTimeStep();
		}
		else if(m_body->isContact(WEST) || m_body->isContact(EAST)) // Wall jumping
		{
			m_body->setVelocityY(m_body->getVelocity().y*0.5f);
			if(Input::getKeyState(XD_KEY_SPACE))
			{
				if(m_canJump)
				{
					m_body->setVelocityX((m_body->isContact(WEST) - m_body->isContact(EAST)) * 14.0f);
					m_body->setVelocityY(-4.5f);
					m_jumpTimer = 0.0f;
					m_canJump = false;
				}
			}
			else
			{
				m_canJump = true;
			}
		}
	}

	// Walking
	if(abs(m_body->getVelocity().x) < 10.0f)
	{
		m_body->applyImpulse(Vector2((Input::getKeyState(XD_KEY_D) - Input::getKeyState(XD_KEY_A)) * (Input::getKeyState(XD_KEY_SHIFT) ? 1.0f : 0.5f), 0.0f));
	}

	// Apply friction
	m_body->setVelocityX(m_body->getVelocity().x * 0.85f);
	
	// Set animations
	m_walkAnimationState->setTimeScale(math::abs(m_body->getVelocity().x) * 0.5f);
	if(m_body->isContact(SOUTH))
	{
		m_walkAnimationState->setLooping(true);
		if(m_body->getVelocity().x >= 0.01f)
		{
			changeAnimation("walk");
			m_skeleton->setFlipX(false);
		}
		else if(m_body->getVelocity().x <= -0.01f)
		{
			changeAnimation("walk");
			m_skeleton->setFlipX(true);
		}
		else
		{
			changeAnimation("idle");
			m_body->setVelocityX(0.0f);
			m_walkAnimationState->setTimeScale(1.0f);
		}
	}
	else
	{
		if(m_body->isContact(WEST)/* >= 3*/) // TODO: I should check for a column of 3 rows of blocks instead of simlply one
		{
			m_skeleton->setFlipX(false);
			m_walkAnimationState->setLooping(false);
			m_walkAnimationState->setTimeScale(5.0f);
			changeAnimation("wall-slide");
		}
		else if(m_body->isContact(EAST))
		{
			m_skeleton->setFlipX(true);
			m_walkAnimationState->setLooping(false);
			m_walkAnimationState->setTimeScale(5.0f);
			changeAnimation("wall-slide");
		}
		else
		{
			m_walkAnimationState->setLooping(false);
			m_walkAnimationState->setTimeScale(1.0f);
			changeAnimation("jump");
		}
	}

	// Update physics
	m_body->update();

	// Use current item
	ItemData *item = ItemData::get(m_itemContainer.getItemAt(m_selectedItemSlot));
	if(Input::getKeyState(XD_LMB) && item != nullptr)
	{
		item->use(this);
		Animation *anim = m_skeleton->findAnimation("mine");
		if(anim != m_itemAnimation)
		{
			m_itemAnimationState->setAnimation(anim);
			m_itemAnimation = anim;
		}
	}
	else
	{
		m_itemAnimation = nullptr;
	}

	// Move camera
	m_camera->lookAt(m_body->getPosition());

	// Update animations
	m_skeleton->setPosition(m_body->getPosition() + Vector2(m_body->getSize().x*0.5f, 48.0f));
	m_walkAnimationState->update(Graphics::getTimeStep());
	if(m_itemAnimation) m_itemAnimationState->update(Graphics::getTimeStep());
}

void Player::draw(SpriteBatch *spriteBatch)
{
	m_skeleton->draw(spriteBatch);
	
	ItemData *item = ItemData::get(m_itemContainer.getItemAt(m_selectedItemSlot));
	if(item != nullptr)
	{
		item->draw(this, spriteBatch);
	}
}