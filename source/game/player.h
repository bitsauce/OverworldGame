#ifndef PLAYER_H
#define PLAYER_H

#include "Config.h"
#include "Game/GameObject.h"

class Skeleton;
class AnimationStateData;
class AnimationState;
class Animation;
class Camera;
class PhysicsBody;
class Terrain;
class ItemData;

class Player : public GameObject
{
public:
	Player();

	void update();
	void draw(SpriteBatch *SpriteBatch);
	void changeAnimation(const string &name);

	PhysicsBody *getBody() const { return m_body; }
	Skeleton *getSkeleton() const { return m_skeleton; }
	Camera *getCamera() const { return m_camera; }
	Terrain *getTerrain() const { return m_terrain; }

private:

	Camera *m_camera;

	Terrain *m_terrain;

	ItemData *m_currentItem;

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