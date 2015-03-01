#ifndef PLAYER_H
#define PLAYER_H

#include "Config.h"
#include "Game/GameObject.h"
#include "Game/ItemContainer.h"
#include "Networking/NetworkObject.h"
#include "Entity.h"
#include "Humanoid.h"
#include "Physics/PhysicsBody.h"

class Camera;
class Terrain;
class ItemData;
class ItemContainer;
class Connection;

class Player : public Entity, public NetworkObject
{
public:
	Player(RakNet::RakNetGUID guid);

	void update();
	void pack(RakNet::BitStream *bitStream, const Connection *conn);
	void unpack(RakNet::BitStream *bitStream, const Connection *conn);
	void draw(SpriteBatch *spriteBatch);

	uint getMaxHealth() const { return m_maxHealth; }
	uint getHealth() const { return m_health; }
	
	void setSelectedItemSlot(const uint slot) { m_selectedItemSlot = slot; }
	uint getSelectedItemSlot() const { return m_selectedItemSlot; }

	Camera *getCamera() const { return m_camera; }
	Terrain *getTerrain() const { return m_terrain; }

	ItemContainer &getItemContainer() { return m_itemContainer; }
	Humanoid &getHumanoid() { return m_humanoid; }
	PhysicsBody &getBody() { return m_body; }

private:
	// Managers
	Camera *m_camera;
	Terrain *m_terrain;

	// Inventory
	ItemContainer m_itemContainer;
	uint m_selectedItemSlot;

	// Player health
	uint m_maxHealth;
	uint m_health;

	// Player-human model
	Humanoid m_humanoid;

	bool m_lmbPressed;

	RakNet::RakNetGUID m_guid;
	
	enum
	{
		INPUT_MOVE_LEFT,
		INPUT_MOVE_RIGHT,
		INPUT_JUMP,
		INPUT_COUNT
	};
	bool m_inputState[INPUT_COUNT];

	// Physics
	PhysicsBody m_body;
	float m_jumpTimer;
	bool m_canJump;
};

#endif // PLAYER_H