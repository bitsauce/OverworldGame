#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H
//>REMOVE ME<
#include "Config.h"
#include "Entities/Entity.h"

class Terrain;
class World;

class DynamicEntity : public Entity
{
public:
	DynamicEntity(World *world, const EntityID id);

	virtual void update(const float delta);
	//void debugDraw(SpriteBatch *spriteBatch, const float alpha);

	void setPosition(const Vector2 &pos) { m_position = pos; }
	Vector2 getPosition() const { return m_position; }

	void setSize(const Vector2 &size) { m_size = size; }
	Vector2 getSize() const { return m_size; }

	void setVelocity(const Vector2 &velocity) { m_velocity = velocity; }
	Vector2 getVelocity() const { return m_velocity; }

	bool isContact(const uint dir) { return (m_contact & dir) != 0; }
	
	void setGravityScale(const float gs) { m_gravityScale = gs; }
	float getGravityScale() const { return m_gravityScale; }

	void setAcceleration(const Vector2 &acc) { m_acceleration = acc; }
	Vector2 getAcceleration() const { return m_acceleration; }
	
	void applyImpulse(const Vector2 &impulse) { m_acceleration += impulse; }

	Vector2 getDrawPosition(const float alpha) { return math::lerp(m_prevPosition, m_position, alpha); }

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