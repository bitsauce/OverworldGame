#include "player.h"
#include "game/world.h"
#include "game/camera.h"
#include "terrain/terrain.h"
#include "constants.h"
#include "physics/physicsbody.h"

#include "animation/animation.h"
#include "animation/skeleton.h"

#include <stdlib.h>

Player::Player() :
	GameObject(DRAW_ORDER_PLAYER),
	m_camera(World::getCamera()),
	m_jumpTimer(1.0f),
	m_canJump(false),
	m_currentAnim(nullptr)
{
	// Load physics
	m_body = new PhysicsBody();
	m_body->setSize(24, 48);

	// Load skeleton data
	m_skeleton = new Skeleton(":/sprites/characters/anim/skeleton.json", ":/sprites/characters/anim/skeleton.atlas", 1.0f);
	m_skeleton->getTexture()->setFiltering(xd::Texture2D::LINEAR);
	
	// Setup spine animations // TODO: Move to global scope (as only one copy of this is strictly neseccary)
	AnimationStateData *data = new AnimationStateData(m_skeleton);
	data->setMix("idle", "walk", 0.2f);
	data->setMix("walk", "idle", 0.5f);
	data->setMix("jump", "idle", 0.1f);
	data->setMix("walk", "jump", 0.1f);
	data->setMix("jump", "idle", 0.1f);
	data->setMix("idle", "jump", 0.2f);
	
	// Create spine animation
	m_animation = new AnimationState(data);
	//m_animation.setEventCallback(animationEvent);
	m_animation->setLooping(true);
	m_skeleton->setFlipY(true); // TODO: Investigate why this is needed
	changeAnimation("idle");
}

// ANIMATIONS
void Player::changeAnimation(const string &name)
{
	// Get animation by name
	Animation *anim = m_skeleton->findAnimation(name);
	if(anim == nullptr)
	{
		xd::LOG("Humanoid::changeAnimation() - Animation '%s' does not exists.", name);
		return;
	}
		
	// Make sure this animation isn't current
	if(m_currentAnim != anim)
	{
		m_animation->setAnimation(anim);
		m_currentAnim = anim;
	}
}


void Player::update()
{
	// Jumping
	if(m_body->isContact(SOUTH))
	{
		if(xd::Input::getKeyState(xd::XD_KEY_SPACE))
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
			if(xd::Input::getKeyState(xd::XD_KEY_SPACE)) // High/low jumping
			{
				m_body->applyImpulse(Vector2(0.0f, -0.75f));
			}
			m_jumpTimer += xd::Graphics::getTimeStep();
		}
		else if(m_body->isContact(WEST) || m_body->isContact(EAST)) // Wall jumping
		{
			m_body->setVelocityY(m_body->getVelocity().y*0.5f);
			if(xd::Input::getKeyState(xd::XD_KEY_SPACE))
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
		m_body->applyImpulse(Vector2((xd::Input::getKeyState(xd::XD_KEY_D) - xd::Input::getKeyState(xd::XD_KEY_A)) * (xd::Input::getKeyState(xd::XD_KEY_SHIFT) ? 1.0f : 0.5f), 0.0f));
	}

	// Apply friction
	m_body->setVelocityX(m_body->getVelocity().x * 0.85f);
	
	// Set animations
	m_animation->setTimeScale(xd::math::abs(m_body->getVelocity().x) * 0.5f);
	if(m_body->isContact(SOUTH))
	{
		m_animation->setLooping(true);
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
			m_animation->setTimeScale(1.0f);
		}
	}
	else
	{
		if(m_body->isContact(WEST)/* >= 3*/) // TODO: I should check for a column of 3 rows of blocks instead of simlply one
		{
			m_skeleton->setFlipX(false);
			m_animation->setLooping(false);
			m_animation->setTimeScale(5.0f);
			changeAnimation("wall-slide");
		}
		else if(m_body->isContact(EAST))
		{
			m_skeleton->setFlipX(true);
			m_animation->setLooping(false);
			m_animation->setTimeScale(5.0f);
			changeAnimation("wall-slide");
		}
		else
		{
			m_animation->setLooping(false);
			m_animation->setTimeScale(1.0f);
			changeAnimation("jump");
		}
	}

	// Update physics
	m_body->update();

	// Move camera
	m_camera->lookAt(m_body->getPosition());

	m_skeleton->setPosition(m_body->getPosition() + Vector2(m_body->getSize().x*0.5f, 48.0f));
	m_animation->update(xd::Graphics::getTimeStep());
}

void Player::draw(xd::SpriteBatch *SpriteBatch)
{
	m_skeleton->draw(SpriteBatch);
}