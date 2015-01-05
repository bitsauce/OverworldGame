#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include <x2d/x2d.h>

class LightSource
{
public:
	// Virtual draw
	virtual void draw(xd::GraphicsContext &gfxContext) = 0;

	// Source position
	void setPosition(const Vector2 &position);
	Vector2 getPosition() const;

	// Source color
	void setColor(const xd::Color &color);
	xd::Color getColor() const;

protected:
	LightSource(Vector2 position, xd::Color color) : m_position(position), m_color(color) {}

	Vector2 m_position;
	xd::Color m_color;
};

#endif // LIGHT_SOURCE_H