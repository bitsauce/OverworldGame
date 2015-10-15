#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H

#include "Config.h"
#include "../Entity.h"

class Terrain;
class World;

class DynamicEntity : public Entity
{
public:
	DynamicEntity(World *world, const EntityID id);

	virtual void update(const float delta);

	/* Position functions */
	void setPosition(const Vector2 &pos)
	{
		m_lastPosition = m_position = pos;
	}

	void setPosition(const float x, const float y)
	{
		setPosition(Vector2(x, y));
	}

	void setX(const float x)
	{
		setPosition(x, getY());
	}

	void setY(const float y)
	{
		setPosition(getX(), y);
	}

	float getX() const
	{
		return getPosition().x;
	}

	float getY() const
	{
		return getPosition().y;
	}

	Vector2 getPosition() const
	{
		return m_position;
	}

	Vector2 getLastPosition() const
	{
		return m_lastPosition;
	}

	void moveTo(const Vector2 &pos)
	{
		m_lastPosition = m_position;
		m_position = pos;
	}

	/* Size functions */
	void setSize(const Vector2 &size)
	{
		m_size = size;
	}

	Vector2 getSize() const
	{
		return m_size;
	}

	void setSize(const float w, const float h)
	{
		setSize(Vector2(w, h));
	}

	void setWidth(const float w)
	{
		setSize(w, getHeight());
	}

	void setHeight(const float h)
	{
		setSize(getWidth(), h);
	}

	float getWidth() const
	{
		return getSize().x;
	}

	float getHeight() const
	{
		return getSize().y;
	}

	/* Velocity functions */
	void setVelocity(const Vector2 &velocity)
	{
		m_velocity = velocity;
	}

	Vector2 getVelocity() const
	{
		return m_velocity;
	}

	void setVelocity(const float x, const float y)
	{
		setVelocity(Vector2(x, y));
	}

	void setVelocityX(const float x)
	{
		setVelocity(x, getVelocityY());
	}

	void setVelocityY(const float y)
	{
		setVelocity(getVelocityX(), y);
	}

	float getVelocityX() const
	{
		return getVelocity().x;
	}

	float getVelocityY() const
	{
		return getVelocity().y;
	}

	/* Acceleration functions */
	void setAcceleration(const float x, const float y)
	{
		setAcceleration(Vector2(x, y));
	}

	void setAccelerationX(const float x)
	{
		setAcceleration(x, getAccelerationY());
	}

	void setAccelerationY(const float y)
	{
		setAcceleration(getAccelerationX(), y);
	}

	float getAccelerationX() const
	{
		return getAcceleration().x;
	}

	float getAccelerationY() const
	{
		return getAcceleration().y;
	}

	/* Physics functions */
	bool isContact(const uint dir)
	{
		return (m_contact & dir) != 0;
	}

	void setGravityScale(const float gs)
	{
		m_gravityScale = gs;
	}

	float getGravityScale() const
	{
		return m_gravityScale;
	}

	void setAcceleration(const Vector2 &acc)
	{
		m_acceleration = acc;
	}

	Vector2 getAcceleration() const
	{
		return m_acceleration;
	}

	void applyImpulse(const Vector2 &impulse)
	{
		m_acceleration += impulse;
	}

	Vector2 getDrawPosition(const float alpha) const
	{
		return math::lerp(m_lastPosition, m_position, alpha);
	}

	Rect getRect() const
	{
		return Rect(getPosition(), getSize());
	}

	Vector2 getCenter() const
	{
		return getPosition() + getSize() * 0.5f;
	}

	bool m_allowRotation;
private:
	Vector2 m_acceleration;
	Vector2 m_velocity;
	Vector2 m_position;
	Vector2 m_lastPosition;
	Vector2 m_size;
	float m_rotation;
	float m_gravityScale;
	uint m_contact;

	Terrain *m_terrain;
};

#endif // PHYSICS_BODY_H