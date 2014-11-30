#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include <x2d/x2d.h>

class PhysicsBody
{
public:
	PhysicsBody();

	void update();

	void draw(XBatch *);

	void setPosition(const float x, const float y) { m_position.set(x, y); }
	Vector2 getPosition() const { return m_position; }
	void setSize(const float w, const float h) { m_size.set(w, h); }
	Vector2 getSize() const { return m_size; }
	void setVelocity(const Vector2 &velocity) { m_velocity = velocity; }
	void setVelocityX(const float x) { m_velocity.x = x; }
	void setVelocityY(const float y) { m_velocity.y = y; }

private:
	Vector2 m_velocity;
	Vector2 m_position;
	Vector2 m_size;
	float m_rotation;
	float m_gravityScale;
	bool m_allowRotation;

	class Terrain *m_terrain;
};

#endif // PHYSICS_BODY_H