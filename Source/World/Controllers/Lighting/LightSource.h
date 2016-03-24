#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "Config.h"

class LightSource
{
public:
	// Virtual draw
	virtual void draw(SpriteBatch *spriteBatch) = 0;

	// Source position
	void setPosition(const Vector2F &position);
	Vector2F getPosition() const;

	// Source color
	void setColor(const Color &color);
	Color getColor() const;

	// Pointlight radius
	void setRadius(const float radius);
	float getRadius() const;

protected:
	LightSource(Vector2F position, float radius, Color color) : m_position(position), m_radius(radius), m_color(color) {}

	Vector2F m_position;
	float m_radius;
	Color m_color;
};

#endif // LIGHT_SOURCE_H