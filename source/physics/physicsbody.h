#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include "Config.h"

class Terrain;
class World;

class PhysicsBody
{
public:
	PhysicsBody(World &world);

	void update();

	void draw(SpriteBatch *);

	void setPosition(const float x, const float y) { m_position.set(x, y); }
	void setPosition(const Vector2 &pos) { m_position = pos; }
	Vector2 getPosition() const { return m_position; }
	void setSize(const float w, const float h) { m_size.set(w, h); }
	Vector2 getSize() const { return m_size; }
	void setVelocity(const Vector2 &velocity) { m_velocity = velocity; }
	Vector2 getVelocity() const { return m_velocity; }
	void setVelocityX(const float x) { m_velocity.x = x; }
	void setVelocityY(const float y) { m_velocity.y = y; }
	Vector2 getCenter() const { return m_position + m_size * 0.5f; }
	Rect getRect() const { return Rect(m_position, m_size); }
	bool isContact(const uint dir) { return (m_contact & dir) != 0; }
	void setGravityScale(const float gs) { m_gravityScale = gs; }

	void applyImpulse(const Vector2 &impulse) { m_acceleration += impulse; }
	void setAccelerationX(const float ax) { m_acceleration.x = ax; }
	void setAccelerationY(const float ay) { m_acceleration.y = ay; }

	Vector2 getDrawPosition(const float alpha) {
		return Vector2(math::lerp(m_prevPosition.x, m_position.x, alpha), math::lerp(m_prevPosition.y, m_position.y, alpha));
	}

private:

	Vector2 m_acceleration;
	Vector2 m_velocity;
	Vector2 m_position;
	Vector2 m_prevPosition;
	Vector2 m_size;
	float m_rotation;
	float m_gravityScale;
	bool m_allowRotation;
	uint m_contact;

	Terrain *m_terrain;
};

#endif // PHYSICS_BODY_H