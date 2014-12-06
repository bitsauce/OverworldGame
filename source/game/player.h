#ifndef PLAYER_H
#define PLAYER_H

#include <x2d/x2d.h>

#include "game/gameobject.h"

class Player : public GameObject
{
public:
	Player();

	void update();
	void draw(XBatch *batch);

	class PhysicsBody *m_body;

private:

	XSprite *m_sprite;
	class Camera *m_camera;
	float m_jumpTimer;
};

#endif // PLAYER_H