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
	void draw(xd::SpriteBatch *SpriteBatch);
	void changeAnimation(const string &name);

	class PhysicsBody *m_body;

private:

	Camera *m_camera;
	float m_jumpTimer;
	bool m_canJump;
	
	// Skeletal animations
	Skeleton *m_skeleton;
	AnimationState *m_animation;
	Animation *m_currentAnim;
};

#endif // PLAYER_H