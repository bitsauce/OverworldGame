#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "Config.h"

class World;

class LightSource
{
public:
	// Virtual draw
	//virtual void draw(SpriteBatch *spriteBatch) = 0;

	// Source position
	void setPosition(const Vector2F &position);
	Vector2F getPosition() const;

	// Source color
	void setColor(const Color &color);
	Color getColor() const;

	// Pointlight radius
	void setRadius(const float radius);
	float getRadius() const;

	enum Type
	{
		STATIC,
		DYNAMIC
	};

	Type getType() const { return m_type; }

protected:
	LightSource(World *world, Type type, Vector2F position, float radius, Color color);

	Vector2F m_position;
	float m_radius;
	Color m_color;
	const Type m_type;
};

#endif // LIGHT_SOURCE_H