#include "LightSource.h"
#include "World/World.h"

LightSource::LightSource(World *world, const Type type) :
	m_world(world),
	m_type(type),
	m_position(Vector2F(0.0f)),
	m_radius(10.0f),
	m_color(Color(255)),
	m_mobility(DYNAMIC)
{
	m_world->getLighting()->addLightSource(this);
}

LightSource::~LightSource()
{
	m_world->getLighting()->removeLightSource(this);
}

void LightSource::setPosition(const Vector2F &position)
{
	m_position = position;
	onModified();
}

Vector2F LightSource::getPosition() const
{
	return m_position;
}

void LightSource::setColor(const Color &color)
{
	m_color = color;
	onModified();
}

Color LightSource::getColor() const
{
	return m_color;
}

void LightSource::setRadius(const float radius)
{
	m_radius = radius;
	onModified();
}

float LightSource::getRadius() const
{
	return m_radius;
}

void LightSource::onModified()
{
	// Set redraw to true if a static light was modified
	if(m_type == STATIC)
	{
		m_world->getLighting()->m_redrawStaticLighting = true;
	}
}