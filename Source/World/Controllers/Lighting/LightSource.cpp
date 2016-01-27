#include "LightSource.h"

// TODO: Fix lighting bug where moving a lightsource should detach the lighting
// on all the chunks affected by this light.
void LightSource::setPosition(const Vector2F &position)
{
	m_position = position;
}

Vector2F LightSource::getPosition() const
{
	return m_position;
}

void LightSource::setColor(const Color &color)
{
	m_color = color;
}

Color LightSource::getColor() const
{
	return m_color;
}

void LightSource::setRadius(const float radius)
{
	m_radius = radius;
}

float LightSource::getRadius() const
{
	return m_radius;
}