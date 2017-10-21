#pragma once

#include "Config.h"
#include "Networking/NetworkObject.h"

class World;
class EntityData;
class Connection;
class Chunk;

#define DECLARE_ENTITY(EntityName) \
friend class EntityData; \
friend class World; \
friend class Connection; \
private: \
	EntityName(World *world, const Json::Value &attributes); \
	~EntityName(); \
	static Entity *Factory(World *world, const Json::Value &attributes) { return new EntityName(world, attributes); } \
	const EntityData *getData() const { return EntityData::GetByName(#EntityName); }

class Entity : public SceneObject, public RakNet::NetworkIDObject
{
public:
	Entity(World *world, const Json::Value &attributes);
	virtual ~Entity();

	virtual void onTick(TickEvent *e);

	virtual void onSaveData(FileWriter &f) {}
	virtual void onLoadData(FileReader &f) {}

	virtual const EntityData *getData() const = 0;

	/* Position functions */
	void setPosition(const Vector2F &pos);
	void setPosition(const float x, const float y) { setPosition(Vector2F(x, y)); }

	void setX(const float x) { setPosition(x, getY()); }
	void setY(const float y) { setPosition(getX(), y); }

	float getX() const { return getPosition().x; }
	float getY() const { return getPosition().y; }

	Vector2F getPosition() const { return m_position; }
	Vector2F getLastPosition() const { return m_lastPosition; }

	void moveTo(const Vector2F &pos)
	{
		m_lastPosition = m_position;
		m_position = pos;
	}

	/* Size functions */
	void setSize(const Vector2F &size)
	{
		m_size = size;
	}

	Vector2F getSize() const
	{
		return m_size;
	}

	void setSize(const float w, const float h)
	{
		setSize(Vector2F(w, h));
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
	void setVelocity(const Vector2F &velocity)
	{
		m_velocity = velocity;
	}

	Vector2F getVelocity() const
	{
		return m_velocity;
	}

	void setVelocity(const float x, const float y)
	{
		setVelocity(Vector2F(x, y));
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
		setAcceleration(Vector2F(x, y));
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

	bool isLastContact(const uint dir)
	{
		return (m_lastContact & dir) != 0;
	}

	void setGravityScale(const float gs)
	{
		m_gravityScale = gs;
	}

	float getGravityScale() const
	{
		return m_gravityScale;
	}

	void setAcceleration(const Vector2F &acc)
	{
		m_acceleration = acc;
	}

	Vector2F getAcceleration() const
	{
		return m_acceleration;
	}

	void applyImpulse(const Vector2F &impulse)
	{
		m_acceleration += impulse;
	}

	Vector2F getDrawPosition(const float alpha) const
	{
		return math::lerp(m_lastPosition, m_position, alpha);
	}

	RectF getRect() const
	{
		return RectF(getPosition(), getSize());
	}

	Vector2F getCenter() const
	{
		return getPosition() + getSize() * 0.5f;
	}

	virtual void packData(RakNet::BitStream *bitStream) { }
	virtual bool unpackData(RakNet::BitStream *bitStream, const bool force) { return true; }

	RakNet::RakNetGUID getOriginGUID() const
	{
		return m_originGUID;
	}

	void setOriginGUID(RakNet::RakNetGUID originGUID);

	bool isClientObject() const
	{
		return m_isClientObject;
	}

	bool m_allowRotation;

protected:
	World * const m_world;
	Connection *const m_connection;

private:
	Vector2F m_acceleration;
	Vector2F m_velocity;
	Vector2F m_position;
	Vector2F m_lastPosition;
	Vector2F m_size;
	Chunk *m_currentChunk;
	float m_rotation;
	float m_gravityScale;
	uint m_contact, m_lastContact;

	RakNet::RakNetGUID m_originGUID;
	bool m_isClientObject;
};