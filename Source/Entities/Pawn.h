#ifndef PAWN_H
#define PAWN_H

#include "Config.h"
#include "Game/Storage.h"
#include "Humanoid.h"
#include "Entity.h"
#include "Networking/Controller.h"

class Camera;
class Terrain;
class ItemData;
class Connection;
class Overworld;
class GameOverlay;
class Pointlight;
class Spotlight;
class Item;

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

class Pawn : public Entity
{
public:
	Pawn(World *world, const Json::Value &attributes);
	virtual ~Pawn();

	void setController(Controller *controller);
	Controller *getController() const
	{
		return m_controller;
	}

	void setSelectedSlot(const int slot);

	int getSelectedSlot() const
	{
		return m_selectedSlot;
	}

	void activateThing(int action);

	virtual void onTick(TickEvent *e);
	virtual void onDraw(DrawEvent *e);

	void packData(RakNet::BitStream *bitStream);
	void unpackData(RakNet::BitStream *bitStream);

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

	// Inventory
	Storage::Slot m_heldItem;
	Bag *m_bag;
	Storage m_storage;
	int m_selectedSlot;

	// Equiped item
	ItemID m_prevEquipedItemID, m_equipedItemID;
	Item *m_equipedItem;

	// Pawn health
	int m_maxHealth;
	int m_health;

	// Pawn-human model
	Humanoid m_humanoid;

	// Physics
	float m_jumpTimer;
	bool m_canJump;

	float m_jumpForce;
	float m_jumpEase;
	float m_maxSpeed;
	float m_moveSpeed;
	float m_friction;

	friend class Commander;

	bool m_lmbPressed;
public:
	//Pointlight *m_pointlight;
	Spotlight *m_spotlight;
};

#endif // PAWN_H