#include "LightSource.h"

void LightSource::setPosition(const Vector2 &position)
{
	m_position = position;
}

Vector2 LightSource::getPosition() const
{
	return m_position;
}

void LightSource::setColor(const xd::Color &color)
{
	m_color = color;
}

xd::Color LightSource::getColor() const
{
	return m_color;
}