#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include "Config.h"

class LightSource
{
public:
	// Virtual draw
	virtual void draw(SpriteBatch *spriteBatch) = 0;

	// Source position
	void setPosition(const Vector2 &position);
	Vector2 getPosition() const;

	// Source color
	void setColor(const Color &color);
	Color getColor() const;

	// Pointlight radius
	void setRadius(const float radius);
	float getRadius() const;

protected:
	LightSource(Vector2 position, float radius, Color color) : m_position(position), m_radius(radius), m_color(color) {}

	Vector2i m_position;
	float m_radius;
	Color m_color;
};

#endif // LIGHT_SOURCE_H