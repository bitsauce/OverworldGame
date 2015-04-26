#include "BitStream.h"

#include "Player.h"
#include "World/World.h"
#include "World/Camera.h"
#include "World/Terrain/Terrain.h"
#include "Constants.h"
#include "Entities/Physics/DynamicEntity.h"
#include "Items/ItemData.h"
#include "Networking/Connection.h"
#include "Networking/Server.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Gui/GameOverlay.h"
#include "Things/Thing.h"
#include "Game/Game.h"
#include "Game/GameStates/GameState.h"
#include "Game/Scene.h"

#include "Gui/Canvas.h"

Player::Player(Game *game, RakNet::RakNetGUID guid) :
	DynamicEntity(game->getWorld(), ENTITY_PLAYER),
	m_camera(game->getWorld()->getCamera()),
	m_terrain(game->getWorld()->getTerrain()),
	m_gameOverlay(game->getGameOverlay()),
	m_jumpTimer(1.0f),
	m_canJump(false),
	m_selectedItemSlot(0),
	m_itemContainer(110),
	m_guid(guid),
	m_maxHealth(12),
	m_health(m_maxHealth),
	m_lmbPressed(false)
{
	// Set body size
	//setSize(24, 48);
	Entity::setSize(24, 48);
	
	// If player is local, do extra stuff
	if(Connection::getInstance()->getGUID() == guid)
	{
		game->getGameOverlay()->setPlayer(this);

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
		Input::bind(XD_RMB, bind(&Player::activateThing, this));

		m_camera->setTargetEntity(this);

		m_world->m_localPlayer = this;
	}

	// Add to player list
	m_world->m_players.push_back(this);
}

Player::~Player()
{
	//delete m_gameOverlay;
}

void Player::mouseWheelEvent(const int delta)
{
	if(delta < 0)
	{
		if(m_selectedItemSlot == 9)
		{
			m_selectedItemSlot = 0;
		}
		else
		{
			m_selectedItemSlot += 1;
		}
	}
	else
	{
		if(m_selectedItemSlot == 0)
		{
			m_selectedItemSlot = 9;
		}
		else
		{
			m_selectedItemSlot -= 1;
		}
	}
}

void Player::update(const float delta)
{
	// Jumping
	if(isContact(SOUTH))
	{
		if(m_inputState[INPUT_JUMP])
		{
			if(m_canJump)
			{
				applyImpulse(Vector2(0.0f, -12.0f));
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
			if(m_inputState[INPUT_JUMP]) // High/low jumping
			{
				applyImpulse(Vector2(0.0f, -2.5f));
			}
			m_jumpTimer += delta;
		}
		else if(isContact(WEST) || isContact(EAST)) // Wall jumping
		{
			setVelocityY(getVelocity().y * 0.5f);
			if(m_inputState[INPUT_JUMP])
			{
				if(m_canJump)
				{
					setVelocityX((isContact(WEST) - isContact(EAST)) * 14.0f);
					setVelocityY(-4.5f);
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
	applyImpulse(Vector2((m_inputState[INPUT_MOVE_RIGHT] - m_inputState[INPUT_MOVE_LEFT]) * (Input::getKeyState(XD_KEY_SHIFT) ? 1.5f : 1.0f) * 10.0f, 0.0f));
	if(getVelocity().x < -5.0f)
	{
		setVelocityX(-5.0f);
		setAccelerationX(0.0f);
	}
	else if(getVelocity().x > 5.0f)
	{
		setVelocityX(5.0f);
		setAccelerationX(0.0f);
	}
	else
	{
		// Apply friction
		setVelocityX(getVelocity().x * 0.85f * delta);
	}

	// Update physics
	DynamicEntity::update(delta);

	// Use current item
	if(Input::getKeyState(XD_LMB) && !m_gameOverlay->isHovered())
	{
		ItemData *item = nullptr;
		if(!m_gameOverlay->getHoldItem().isEmpty())
		{
			item = ItemData::get(m_gameOverlay->getHoldItem().item);
		}
		else
		{
			item = ItemData::get(m_itemContainer.getItemAt(m_selectedItemSlot));
		}

		if(item != nullptr && (!item->isSingleShot() || !m_lmbPressed))
		{
			item->use(this, delta);
		}

		m_lmbPressed = true;
	}
	else
	{
		m_humanoid.setPostAnimation(Humanoid::ANIM_NULL);
		m_lmbPressed = false;
	}
	
	// Set animations
	m_humanoid.getMainAnimationState()->setTimeScale(math::abs(getVelocity().x) * 4.0f * delta);
	if(isContact(SOUTH))
	{
		m_humanoid.getMainAnimationState()->setLooping(true);
		if(getVelocity().x >= 0.01f)
		{
			m_humanoid.setMainAnimation(Humanoid::ANIM_WALK);
			m_humanoid.getSkeleton()->setFlipX(false);
		}
		else if(getVelocity().x <= -0.01f)
		{
			m_humanoid.setMainAnimation(Humanoid::ANIM_WALK);
			m_humanoid.getSkeleton()->setFlipX(true);
		}
		else
		{
			m_humanoid.setMainAnimation(Humanoid::ANIM_IDLE);
			setVelocityX(0.0f);
			m_humanoid.getMainAnimationState()->setTimeScale(1.0f);
		}
	}
	else
	{
		if(isContact(WEST)/* >= 3*/) // TODO: I should check for a column of 3 rows of blocks instead of simlply one
		{
			m_humanoid.getSkeleton()->setFlipX(false);
			m_humanoid.getMainAnimationState()->setLooping(false);
			m_humanoid.getMainAnimationState()->setTimeScale(5.0f);
			m_humanoid.setMainAnimation(Humanoid::ANIM_WALL_SLIDE);
		}
		else if(isContact(EAST))
		{
			m_humanoid.getSkeleton()->setFlipX(true);
			m_humanoid.getMainAnimationState()->setLooping(false);
			m_humanoid.getMainAnimationState()->setTimeScale(5.0f);
			m_humanoid.setMainAnimation(Humanoid::ANIM_WALL_SLIDE);
		}
		else
		{
			m_humanoid.getMainAnimationState()->setLooping(false);
			m_humanoid.getMainAnimationState()->setTimeScale(1.0f);
			m_humanoid.setMainAnimation(Humanoid::ANIM_JUMP);
		}
	}

	// Update animations
	m_humanoid.update(delta);
}

void Player::activateThing()
{
	Vector2 inputPosition = m_camera->getInputPosition();
	int blockX = (int)floor(inputPosition.x / BLOCK_PXF), blockY = (int)floor(inputPosition.y / BLOCK_PXF);
	for(Thing *thing : m_terrain->getChunkLoader()->getChunkAt((int)floor(inputPosition.x / CHUNK_PXF), (int)floor(inputPosition.y / CHUNK_PXF)).getThings())
	{
		if(thing->getX() == blockX && thing->getY() == blockY)
		{
			thing->activate(this);
		}
	}
}

void Player::pack(RakNet::BitStream *bitStream, const Connection *conn)
{
	if(conn->isServer())
	{
		bitStream->Write(getPosition().x);
		bitStream->Write(getPosition().y);
		bitStream->Write(getVelocity().x);
		bitStream->Write(getVelocity().y);
	}
	else if(conn->getGUID() == m_guid)
	{
		bitStream->Write(Input::getKeyState(XD_KEY_A));
		bitStream->Write(Input::getKeyState(XD_KEY_D));
		bitStream->Write(Input::getKeyState(XD_KEY_SPACE));
	}
}

void Player::unpack(RakNet::BitStream *bitStream, const Connection *conn)
{
	if(conn->isServer())
	{
		bitStream->Read(m_inputState[INPUT_MOVE_LEFT]);
		bitStream->Read(m_inputState[INPUT_MOVE_RIGHT]);
		bitStream->Read(m_inputState[INPUT_JUMP]);
	}
	else
	{
		float x; bitStream->Read(x);
		float y; bitStream->Read(y);
		Entity::setPosition(x, y);
		bitStream->Read(x);
		bitStream->Read(y);
		setVelocityX(x);
		setVelocityY(y);
	}
}

void Player::draw(SpriteBatch *spriteBatch, const float alpha)
{
	m_humanoid.draw(this, spriteBatch, alpha);
	
	ItemData *item = nullptr;
	if(!m_gameOverlay->getHoldItem().isEmpty())
	{
		item = ItemData::get(m_gameOverlay->getHoldItem().item);
	}
	else
	{
		item = ItemData::get(m_itemContainer.getItemAt(m_selectedItemSlot));
	}

	if(item != nullptr)
	{
		item->draw(this, spriteBatch, alpha);
	}
}