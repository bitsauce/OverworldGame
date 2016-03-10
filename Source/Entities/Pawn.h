#ifndef PAWN_H
#define PAWN_H

#include "Config.h"
#include "Game/Storage.h"
#include "Networking/NetworkObject.h"
#include "Humanoid.h"
#include "DynamicEntity.h"
#include "Controller.h"

class Camera;
class Terrain;
class ItemData;
class Connection;
class OverworldGame;
class GameOverlay;

class Bag
{
public:
	Bag(const uint width, const uint height) :
		m_width(width),
		m_height(height),
		m_itemStorage(width * height)
	{
	}

	Storage *getStorage() { return &m_itemStorage; }

	uint getWidth() const { return m_width; }
	uint getHeight() const { return m_height; }

private:
	Storage m_itemStorage;
	const uint m_width;
	const uint m_height;
};

class Pawn : public DynamicEntity, public NetworkObject
{
public:
	Pawn(World *world, const EntityID id);
	virtual ~Pawn();

	void setController(Controller *controller);
	Controller *getController() const
	{
		return m_controller;
	}

	void setSelectedSlot(const int slot)
	{
		m_selectedSlot = slot;
	}

	int getSelectedSlot() const
	{
		return m_selectedSlot;
	}

	void activateThing(int action);

	virtual void onTick(TickEvent *e);
	virtual void onDraw(DrawEvent *e);

	void pack(RakNet::BitStream *bitStream, const Connection *conn);
	void unpack(RakNet::BitStream *bitStream, const Connection *conn);

	void createSaveData(FileWriter &saveData);
	void loadSaveData(FileReader &saveData);

	void decHealth(int amt);
	uint getMaxHealth() const { return m_maxHealth; }
	uint getHealth() const { return m_health; }

	Humanoid &getHumanoid() { return m_humanoid; }
	Storage::Slot *getHeldItem() { return &m_heldItem; }
	Storage *getStorage() { return &m_storage; }
	Storage::Slot *getCurrentItem();
	
	Bag *getBag() { return m_bag; }

protected:
	// Managers
	Camera *m_camera;
	Terrain *m_terrain;

	// Pawn controller
	Controller *m_controller;

	// Physics
	float m_moveSpeed;

	// Inventory
	Storage::Slot m_heldItem;
	Bag *m_bag;
	Storage m_storage;
	int m_selectedSlot;

	// Equiped item
	ItemID m_prevItem;

	// Pawn health
	int m_maxHealth;
	int m_health;

	// Pawn-human model
	Humanoid m_humanoid;

	// Physics
	float m_jumpTimer;
	bool m_canJump;

	bool m_lmbPressed;

	Timer timer;
};

#endif // PAWN_H