#include "LightSource.h"
#include "World/World.h"

LightSource::LightSource(World *world, Type type, Vector2F position, float radius, Color color) :
	m_type(type),
	m_position(position),
	m_radius(radius),
	m_color(color)
{
	world->getLighting()->addLightSource(this);
}


// TODO: Moving a static light source should cause a redraw
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