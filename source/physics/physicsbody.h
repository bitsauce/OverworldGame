#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include "Config.h"

class Terrain;

class PhysicsBody
{
public:
	PhysicsBody();

	void update();

	void draw(xd::SpriteBatch *);

	void setPosition(const float x, const float y) { m_position.set(x, y); }
	Vector2 getPosition() const { return m_position; }
	void setSize(const float w, const float h) { m_size.set(w, h); }
	Vector2 getSize() const { return m_size; }
	void setVelocity(const Vector2 &velocity) { m_velocity = velocity; }
	Vector2 getVelocity() const { return m_velocity; }
	void setVelocityX(const float x) { m_velocity.x = x; }
	void setVelocityY(const float y) { m_velocity.y = y; }
	bool isContact(const uint dir) { return (m_contact & dir) != 0; }
	
	void applyImpulse(const Vector2 &impulse) { m_acceleration += impulse; }
	void setAccelerationX(const float ax) { m_acceleration.x = ax; }
	void setAccelerationY(const float ay) { m_acceleration.y = ay; }

private:
	Vector2 m_acceleration;
	Vector2 m_velocity;
	Vector2 m_position;
	Vector2 m_size;
	float m_rotation;
	float m_gravityScale;
	bool m_allowRotation;
	uint m_contact;
	Terrain *m_terrain;
};

#endif // PHYSICS_BODY_H