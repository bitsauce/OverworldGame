#ifndef STATIC_ENTITY_H
#define STATIC_ENTITY_H

#include "Config.h"
#include "../Entity.h"

class World;

class StaticEntity : public Entity
{
public:
	StaticEntity(World *world, const EntityID id);

	void setPosition(const Vector2i &pos)
	{
		m_position = pos;
	}

	Vector2 getPosition() const
	{
		return m_position;
	}

	void setPosition(const int x, const int y)
	{
		setPosition(Vector2i(x, y));
	}

	void setSize(const Vector2 &size)
	{
		m_size = size;
	}

	Vector2 getSize() const
	{
		return m_size;
	}

	void setVelocity(const Vector2 &velocity)
	{

	}

	Vector2 getVelocity() const
	{
		return Vector2();
	}

	void setAcceleration(const Vector2 &acc)
	{

	}

	Vector2 getAcceleration() const
	{
		return Vector2();
	}

	Vector2 getDrawPosition(const float alpha) const
	{
		return Vector2();
	}

private:
	Vector2i m_position, m_size;
};

#endif // STATIC_ENTITY_H