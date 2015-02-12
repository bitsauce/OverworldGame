#ifndef PLAYER_H
#define PLAYER_H

#include "Config.h"
#include "Game/GameObject.h"

#include "Game/ItemContainer.h"

class Skeleton;
class AnimationStateData;
class AnimationState;
class Animation;
class Camera;
class PhysicsBody;
class Terrain;
class ItemData;
class ItemContainer;

class Player : public GameObject
{
public:
	Player();

	void update();
	void draw(SpriteBatch *spriteBatch);
	void changeAnimation(const string &name);
	
	void setSelectedItemSlot(const int slot) { m_selectedItemSlot = slot; }
	int getSelectedItemSlot() const { return m_selectedItemSlot; }

	PhysicsBody *getBody() const { return m_body; }
	Skeleton *getSkeleton() const { return m_skeleton; }
	Camera *getCamera() const { return m_camera; }
	Terrain *getTerrain() const { return m_terrain; }
	ItemContainer &getItemContainer() { return m_itemContainer; }

private:

	Camera *m_camera;

	Terrain *m_terrain;

	ItemContainer m_itemContainer;
	int m_selectedItemSlot;

	// Physics
	PhysicsBody *m_body;
	float m_jumpTimer;
	bool m_canJump;

	// Skeletal animations
	Skeleton *m_skeleton;
	AnimationStateData *m_animationStateData;
	AnimationState *m_walkAnimationState;
	Animation *m_currentAnim;
	AnimationState *m_itemAnimationState;
	Animation *m_itemAnimation;
};

#endif // PLAYER_H