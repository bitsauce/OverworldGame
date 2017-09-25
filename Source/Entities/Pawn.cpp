#include "BitStream.h"

#include "Pawn.h"
#include "World/World.h"
#include "Constants.h"
#include "Items/ItemData.h"
#include "Networking/Connection.h"
#include "Networking/Server.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Game/Game.h"
#include "Game/States/GameState.h"
#include "Gui/Gui.h"

Pawn::Pawn(World *world, const string &entityName, const Json::Value &attributes) :
	Entity(world, entityName, attributes),
	m_camera(m_world->getCamera()),
	m_terrain(m_world->getTerrain()),
	m_controller(0),
	m_jumpTimer(1.0f),
	m_canJump(false),
	m_maxHealth(12),
	m_health(m_maxHealth),
	m_lmbPressed(false),
	m_storage(10),
	m_bag(new Bag(20, 5)),
	m_prevItemID(ITEM_NONE),
	m_equipedItemID(ITEM_NONE),
	m_selectedSlot(0),
	m_jumpForce(20.0f),
	m_jumpEase(1.5f),
	m_moveSpeed(3.5f),
	m_maxSpeed(7.5f),
	m_friction(0.85f)
{
	/*m_pointlight = new Pointlight(world);
	m_pointlight->setMobility(LightSource::DYNAMIC);
	m_pointlight->setRadius(5.0f);
	m_pointlight->setColor(Color::White);*/

	m_spotlight = new Spotlight(m_world);
	m_spotlight->setMobility(LightSource::DYNAMIC);
	m_spotlight->setRadius(5.0f);
	m_spotlight->setColor(Color::White);
	m_spotlight->setConeAngle(PI / 4.0f);

	// Set body size
	setSize(24, 48);

	// Init inventory things
	m_storage.setNext(m_bag->getStorage());

	// Add to player list
	m_world->m_pawns.push_back(this);
}

Pawn::~Pawn()
{
	delete m_controller;
}

void Pawn::setController(Controller *controller)
{
	m_controller = controller;
	m_controller->setPawn(this);
}

void Pawn::setSelectedSlot(const int slot)
{
	m_selectedSlot = slot;
}

Storage::Slot *Pawn::getCurrentItem()
{
	if(!m_heldItem.isEmpty())
	{
		return &m_heldItem;
	}
	return m_storage.getSlotAt(m_selectedSlot);
}

void Pawn::activateThing(int action)
{
	/*if(action != GLFW_PRESS) return;
	Vector2F inputPosition = m_camera->getInputPosition();
	int blockX = (int)floor(inputPosition.x / BLOCK_PXF), blockY = (int)floor(inputPosition.y / BLOCK_PXF);
	for(Thing *thing : m_terrain->getChunkLoader()->getChunkAt((int)floor(inputPosition.x / CHUNK_PXF), (int)floor(inputPosition.y / CHUNK_PXF)).getThings())
	{
		if(thing->getX() == blockX && thing->getY() == blockY)
		{
			thing->activate(this);
		}
	}*/
}

void Pawn::onTick(TickEvent *e)
{
	if(!m_controller) return;
	m_controller->update();

	// Jumping
	if(isContact(SOUTH))
	{
		if(m_controller->getInputState(Controller::INPUT_JUMP))
		{
			if(m_canJump)
			{
				applyImpulse(Vector2F(0.0f, -m_jumpForce));
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
		if(m_jumpTimer < 0.25f)
		{
			if(m_controller->getInputState(Controller::INPUT_JUMP)) // High/low jumping
			{
				applyImpulse(Vector2F(0.0f, -m_jumpEase));
			}
			m_jumpTimer += e->getDelta();
		}
		/*else if(isContact(WEST) || isContact(EAST)) // Wall jumping
		{
			setVelocityY(getVelocity().y * 0.5f);
			if(m_controller->getInputState(Controller::INPUT_JUMP))
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
		}*/
	}

	// Walking
	float maxSpeed = (m_controller->getInputState(Controller::INPUT_RUN) ? 1.5f : 1.0f) * m_maxSpeed;
	applyImpulse(Vector2F((m_controller->getInputState(Controller::INPUT_MOVE_RIGHT) - m_controller->getInputState(Controller::INPUT_MOVE_LEFT)) * (m_controller->getInputState(Controller::INPUT_RUN) ? 1.5f : 1.0f) * m_moveSpeed, 0.0f));
	if(getVelocity().x < -maxSpeed)
	{
		setVelocityX(-maxSpeed);
		setAccelerationX(0.0f);
	}
	else if(getVelocity().x > maxSpeed)
	{
		setVelocityX(maxSpeed);
		setAccelerationX(0.0f);
	}
	else
	{
		// Apply friction
		setVelocityX(getVelocity().x * m_friction);
	}

	// Update physics
	Entity::onTick(e);
	
	// Set animations
	m_humanoid.getMainAnimationState()->setTimeScale(math::abs(getVelocity().x) * 0.1f);
	if(isLastContact(SOUTH))
	{
		m_humanoid.getMainAnimationState()->setLooping(true);
		if(m_controller->getInputState(Controller::INPUT_MOVE_RIGHT))
		{
			m_humanoid.setMainAnimation(Humanoid::ANIM_WALK);
			m_humanoid.getSkeleton()->setFlipX(false);
		}
		else if(m_controller->getInputState(Controller::INPUT_MOVE_LEFT))
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
		/*if(isContact(WEST) >= 3) // TODO: I should check for a column of 3 rows of blocks instead of simlply one
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
		else*/
		{ // TODO: Need a land animation
			m_humanoid.getMainAnimationState()->setLooping(false);
			m_humanoid.getMainAnimationState()->setTimeScale(1.0f);
			m_humanoid.setMainAnimation(Humanoid::ANIM_JUMP);
		}
	}

	// Item swaped?
	ItemID currentItemID = isClientObject() ? getCurrentItem()->getItem() : m_equipedItemID;
	if(m_prevItemID != currentItemID)
	{
		ItemData *item = ItemData::get(m_prevItemID);
		if(item) item->unequip(this);
		item = ItemData::get(currentItemID);
		if(item) item->equip(this);
		m_prevItemID = currentItemID;
	}

	// Use current item
	ItemData *item = ItemData::get(currentItemID);
	if(item)
	{
		if(m_controller->getInputState(Controller::INPUT_USE_ITEM))
		{
			if(!m_lmbPressed)
			{
				item->use(this, e->getDelta());
			}
			m_lmbPressed = true;
		}
		else
		{
			//m_humanoid.setPostAnimation(Humanoid::ANIM_NULL);
			m_lmbPressed = false;
		}
		item->update(m_world, this, e->getDelta());
	}

	// Update animations // NOTE: Seems wierd to call onTick here...
	m_humanoid.onTick(e);
}

void Pawn::onDraw(DrawEvent *e)
{
	//m_pointlight->setPosition(math::lerp(getLastPosition() + getSize() * 0.5f, getPosition() + getSize() * 0.5f, e->getAlpha()) / BLOCK_PXF);
	m_spotlight->setPosition(math::lerp(getLastPosition() + getSize() * 0.5f, getPosition() + getSize() * 0.5f, e->getAlpha()) / BLOCK_PXF);
	m_spotlight->setDirection((Vector2F(m_world->getCamera()->getInputPosition()) - getCenter()).angle());


	SpriteBatch *spriteBatch = (SpriteBatch*)e->getUserData();
	m_humanoid.draw(this, e->getGraphicsContext(), spriteBatch, e->getAlpha());
	ItemData *item = ItemData::get(getCurrentItem()->getItem());
	if(item)
	{
		item->draw(this, spriteBatch, e->getAlpha());
	}
}

void Pawn::packData(RakNet::BitStream *bitStream, const Connection *conn)
{
	bitStream->Write(getPosition().x);
	bitStream->Write(getPosition().y);
	bitStream->Write(getVelocity().x);
	bitStream->Write(getVelocity().y);
	bitStream->Write(getCurrentItem()->getItem());
}

Timer t;

bool Pawn::unpackData(RakNet::BitStream *bitStream, const Connection *conn)
{
	double time = t.getElapsedTime();
	t.stop();

	// Get position and velocity
	Vector2F position, velocity;
	bitStream->Read(position.x);
	bitStream->Read(position.y);
	bitStream->Read(velocity.x);
	bitStream->Read(velocity.y);
	bitStream->Read(m_equipedItemID);

	// TODO: Verify using some time detla between this and previous packet
	float radius = getVelocity().length() * time * 100.0 + 20.0 /*gravity*/;
	float moved = (getPosition() - position).length();
	if(conn->isServer() && radius < moved)
	{
		// Invalid position, lets not accept the values we we're sent
		// Send back a packet containing the server-side object state
		// to all clients
		return false;
	}
	else
	{
		// Lets move the player to their new (verified) position
		setPosition(getLastPosition()); // Doing this ensures that players are interpolated correctly from their last position to their new position
		moveTo(position); // TODO: Not really a pretty solution
		setVelocity(velocity);

	}
	t.start();
	return true;
}

void Pawn::createSaveData(FileWriter &saveData)
{
	// Save position
	saveData << (int)getPosition().x << endl;
	saveData << (int)getPosition().y << endl;

	// Save hotbar
	for(uint i = 0; i < 10; ++i)
	{
		saveData << m_storage.getSlotAt(i)->getItem() << endl;
		saveData << m_storage.getSlotAt(i)->getAmount() << endl;
	}

	// Save bag and its content
	saveData << (int)m_bag->getWidth() << endl;
	saveData << (int)m_bag->getHeight() << endl;
	for(uint y = 0; y < m_bag->getHeight(); ++y)
	{
		for(uint x = 0; x < m_bag->getWidth(); ++x)
		{
			saveData << m_bag->getStorage()->getSlotAt(x + y * m_bag->getWidth())->getItem() << endl;
			saveData << m_bag->getStorage()->getSlotAt(x + y * m_bag->getWidth())->getAmount() << endl;
		}
	}
}

void Pawn::loadSaveData(FileReader &saveData)
{
	// Restore position
	int x, y;
	saveData >> x;
	saveData >> y;
	setPosition(Vector2F((float) x, (float) y));

	// Restore hotbar
	for(uint i = 0; i < 10; ++i)
	{
		int item, amount;
		saveData >> item;
		saveData >> amount;
		m_storage.getSlotAt(i)->set((ItemID)item, amount);
	}
	
	// Restore bag and its content
	int bagWidth, bagHeight;
	saveData >> bagWidth;
	saveData >> bagHeight;

	delete m_bag;
	m_bag = new Bag(bagWidth, bagHeight);
	for(int y = 0; y < bagHeight; ++y)
	{
		for(int x = 0; x < bagWidth; ++x)
		{
			int item, amount;
			saveData >> item;
			saveData >> amount;
			m_bag->getStorage()->getSlotAt(x + y * bagWidth)->set((ItemID)item, amount);
		}
	}
}

void Pawn::decHealth(int amt)
{
	m_health -= amt;
	if(m_health <= 0)
	{
		LOG("Dead");
	}
}
