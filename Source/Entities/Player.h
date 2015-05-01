#ifndef PLAYER_H
#define PLAYER_H

#include "Config.h"
#include "Game/ItemContainer.h"
#include "Networking/NetworkObject.h"
#include "Humanoid.h"
#include "Entities/Physics/DynamicEntity.h"

class Camera;
class Terrain;
class ItemData;
class Connection;
class Game;
class GameOverlay;

class Bag
{
public:
	Bag(const uint width, const uint height) :
		m_width(width),
		m_height(height),
		m_itemContainer(width * height)
	{
	}

	ItemContainer *getItemContainer() { return &m_itemContainer; }

private:
	ItemContainer m_itemContainer;
	const uint m_width;
	const uint m_height;
};

class Player : public DynamicEntity, public NetworkObject
{
public:
	Player(Game *game, RakNet::RakNetGUID guid);
	~Player();

	void activateThing();

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	void pack(RakNet::BitStream *bitStream, const Connection *conn);
	void unpack(RakNet::BitStream *bitStream, const Connection *conn);

	uint getMaxHealth() const { return m_maxHealth; }
	uint getHealth() const { return m_health; }

	Humanoid &getHumanoid() { return m_humanoid; }
	
	ItemSlot &getHeldItem() { return m_heldItem; }
	Bag *getBag() { return m_bag; }
	ItemContainer *getHotbarContainer() { return &m_hotbar; }

	ItemSlot &getCurrentItem();

private:
	// Managers
	Camera *m_camera;
	Terrain *m_terrain;
	GameOverlay *m_gameOverlay;

	// Inventory
	ItemSlot m_heldItem;
	Bag *m_bag;
	ItemContainer m_hotbar;

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
	float m_jumpTimer;
	bool m_canJump;
};

#endif // PLAYER_H