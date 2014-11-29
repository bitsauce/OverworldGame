#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include <x2d/x2d.h>

#include "game/gameobject.h"

class PhysicsBody : public GameObject
{
public:
	PhysicsBody();

	void update();

	void draw(XBatch *);

	void setPosition(const float x, const float y) { m_position.set(x, y); }
	void setSize(const float w, const float h) { m_size.set(w, h); }

private:
	Vector2 m_velocity;
	Vector2 m_position;
	Vector2 m_size;
	float m_rotation;
	float m_gravityScale;
	class Terrain *m_terrain;
};

#endif // PHYSICS_BODY_H