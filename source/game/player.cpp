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
	m_jumpTimer(1.0f)
{
	// Load physics
	m_body = new PhysicsBody();
	m_body->setSize(32, 48);

	// Load skeleton data
	m_skeleton = new Skeleton(":/sprites/characters/anim/skeleton.json", ":/sprites/characters/anim/skeleton.atlas", 1.0f);
	m_skeleton->getTexture()->setFiltering(XTexture::LINEAR);
	
	// Setup spine animations // TODO: Move to global scope (as only one copy of this is strictly neseccary)
	AnimationStateData *data = new AnimationStateData(m_skeleton);
	data.setMix("idle", "walk", 0.2f);
	data.setMix("walk", "idle", 0.5f);
	data.setMix("jump", "idle", 0.1f);
	data.setMix("walk", "jump", 0.1f);
	data.setMix("jump", "idle", 0.1f);
	data.setMix("idle", "jump", 0.2f);
	
	// Create spine animation
	m_animation = AnimationState(data);
	//m_animation.setEventCallback(animationEvent);
	m_animation->setLooping(true);
	//changeAnimation("idle");
	
	// TODO: Remove
	m_sprite = new XSprite(XTextureRegion(xd::ResourceManager::get<XTexture>(":/sprites/items/box.png")));
	m_sprite->setSize(32, 48);
}

void Player::update()
{
	// Jumping
	if(m_body->isContact(SOUTH) && XInput::getKeyState(XD_KEY_SPACE))
	{
		m_body->applyImpulse(Vector2(0.0f, -4.5f));
		//m_body->setVelocityY(-10.0f);
		m_jumpTimer = 0.0f;
	}
	else
	{
		if(m_jumpTimer < 0.1f && XInput::getKeyState(XD_KEY_SPACE))
		{
			m_body->applyImpulse(Vector2(0.0f, -0.75f));
			m_jumpTimer += XGraphics::getTimeStep();
		}
	}

	// Walking
	if(abs(m_body->getVelocity().x) < 10.0f)
		m_body->applyImpulse(Vector2((XInput::getKeyState(XD_KEY_D) - XInput::getKeyState(XD_KEY_A)) * (XInput::getKeyState(XD_KEY_Shift) ? 1.0f : 0.5f), 0.0f));

	// Apply friction
	m_body->setVelocityX(m_body->getVelocity().x * 0.85f);

	// Update physics
	m_body->update();

	// Update sprite
	m_sprite->setPosition(m_body->getPosition());

	// Move camera
	m_camera->lookAt(m_body->getPosition());
}

void Player::draw(XBatch *batch)
{
	m_sprite->draw(batch);
}