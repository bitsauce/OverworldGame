#ifndef ENTITY_H
#define ENTITY_H

#include "Config.h"

enum Priority;
enum EntityID;
class World;

class Entity
{
public:
	Entity(World *world, const EntityID id);
	virtual ~Entity();

	const EntityID getID() const { return m_id; }

	virtual void setPosition(const Vector2 &pos) = 0;
	virtual Vector2 getPosition() const = 0;
	void setPosition(const float x, const float y) { setPosition(Vector2(x, y)); }
	void setX(const float x) { setPosition(x, getY()); }
	void setY(const float y) { setPosition(getX(), y); }
	float getX() const { return getPosition().x; }
	float getY() const { return getPosition().y; }

	virtual void setSize(const Vector2 &size) = 0;
	virtual Vector2 getSize() const = 0;
	void setSize(const float w, const float h) { setSize(Vector2(w, h)); }
	void setWidth(const float w) { setSize(w, getHeight()); }
	void setHeight(const float h) { setSize(getWidth(), h); }
	float getWidth() const { return getSize().x; }
	float getHeight() const { return getSize().y; }

	virtual void setVelocity(const Vector2 &vel) = 0;
	virtual Vector2 getVelocity() const = 0;
	void setVelocity(const float x, const float y) { setVelocity(Vector2(x, y)); }
	void setVelocityX(const float x) { setVelocity(x, getVelocityY()); }
	void setVelocityY(const float y) { setVelocity(getVelocityX(), y); }
	float getVelocityX() const { return getVelocity().x; }
	float getVelocityY() const { return getVelocity().y; }
	
	virtual void setAcceleration(const Vector2 &vel) = 0;
	virtual Vector2 getAcceleration() const = 0;
	void setAcceleration(const float x, const float y) { setAcceleration(Vector2(x, y)); }
	void setAccelerationX(const float x) { setAcceleration(x, getAccelerationY()); }
	void setAccelerationY(const float y) { setAcceleration(getAccelerationX(), y); }
	float getAccelerationX() const { return getAcceleration().x; }
	float getAccelerationY() const { return getAcceleration().y; }

	virtual Rect getRect() const { return Rect(getPosition(), getSize()); }
	virtual Vector2 getCenter() const { return getPosition() + getSize() * 0.5f; }

	virtual Vector2 getDrawPosition(const float alpha) = 0;

	virtual void update(const float delta) {}
	virtual void draw(SpriteBatch *spriteBatch, const float alpha) {}

protected:
	World *m_world;

private:
	const EntityID m_id;
};

#endif // ENTITY_H