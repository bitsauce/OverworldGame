#ifndef PLAYER_H
#define PLAYER_H

#include <x2d/x2d.h>

#include "game/gameobject.h"

class Skeleton;
class AnimationState;
class Animation;
class Camera;

class Player : public GameObject
{
public:
	Player();

	void update();
	void draw(XBatch *batch);

	class PhysicsBody *m_body;

private:

	XSprite *m_sprite;
	Camera *m_camera;
	float m_jumpTimer;
	
	// Skeletal animations
	Skeleton *m_skeleton;
	AnimationState *m_animation;
	Animation *m_currentAnim;
};

#endif // PLAYER_H