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
class ItemContainer;
class Connection;
class Game;

class Player : public DynamicEntity, public NetworkObject, public MouseListener
{
public:
	Player(Game *game, RakNet::RakNetGUID guid);
	~Player();

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	void activateThing();

	void pack(RakNet::BitStream *bitStream, const Connection *conn);
	void unpack(RakNet::BitStream *bitStream, const Connection *conn);

	uint getMaxHealth() const { return m_maxHealth; }
	uint getHealth() const { return m_health; }
	
	void setSelectedItemSlot(const uint slot) { m_selectedItemSlot = slot; }
	uint getSelectedItemSlot() const { return m_selectedItemSlot; }

	void setCrafting(const bool crafting) { m_isCrafting = crafting; }
	bool isCrafting() const { return m_isCrafting; }

	ItemContainer &getItemContainer() { return m_itemContainer; }
	Humanoid &getHumanoid() { return m_humanoid; }

	void mouseWheelEvent(const int delta);

private:
	// Managers
	Camera *m_camera;
	Terrain *m_terrain;

	// Inventory
	ItemContainer m_itemContainer;
	uint m_selectedItemSlot;
	bool m_isCrafting;

	class GameOverlay *m_gameOverlay;

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