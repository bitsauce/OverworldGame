#include "player.h"
#include "game/world.h"
#include "game/camera.h"
#include "terrain/terrain.h"
#include "constants.h"
#include "physics/physicsbody.h"

Player::Player() :
	GameObject(DRAW_ORDER_PLAYER)
{
	m_body = new PhysicsBody();
	m_sprite = new XSprite(XTextureRegion(xd::ResourceManager::get<XTexture>(":/sprites/items/box.png")));
	m_sprite->setSize(16, 16);
	m_body->setSize(16, 16);
}

void Player::update()
{
	// Jumping
	if(m_body->isContact(SOUTH) && XInput::getKeyState(XD_KEY_SPACE))
	{
		m_body->setVelocityY(-10.0f);
	}

	// Walking
	m_body->setVelocityX((XInput::getKeyState(XD_KEY_D) - XInput::getKeyState(XD_KEY_A)) * 10.0f);

	// Update physics
	m_body->update();

	// Update sprite
	m_sprite->setPosition(m_body->getPosition());

	// Move camera
	World::getCamera()->lookAt(m_body->getPosition());
}

void Player::draw(XBatch *batch)
{
	m_sprite->draw(batch);
}