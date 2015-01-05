#include "spotlight.h"

xd::Vertex *Spotlight::s_vertices;

Spotlight::Spotlight(const Vector2 &position, const float radius, const xd::Color &color) :
	LightSource(position, color),
	m_radius(radius)
{
	//Lighting::addSpotlight(this);
}

void Spotlight::setRadius(const float radius)
{
	m_radius = radius;
}

float Spotlight::getRadius() const
{
	return m_radius;
}

void Spotlight::draw(xd::GraphicsContext &gfxContext)
{
	s_vertices[0].set4f(xd::VERTEX_POSITION, m_position.x, m_position.y);
	s_vertices[0].set4ub(xd::VERTEX_COLOR, m_color.r, m_color.g, m_color.b, m_color.a);
	s_vertices[0].set4f(xd::VERTEX_TEX_COORD, 0.0f, 1.0f);
	for(uint i = 1; i < SPOTLIGHT_SEGMENTS+2; ++i)
	{
		float r = (2.0f*PI*i)/SPOTLIGHT_SEGMENTS;
		s_vertices[i].set4f(xd::VERTEX_POSITION, m_position.x + cos(r)*m_radius, m_position.y + sin(r)*m_radius);
		s_vertices[i].set4ub(xd::VERTEX_COLOR, m_color.r, m_color.g, m_color.b, 0);
		s_vertices[i].set4f(xd::VERTEX_TEX_COORD, (1 + cos(r))/2.0f, 1.0f - (1 + sin(r))/2.0f);
	}
	gfxContext.drawPrimitives(xd::GraphicsContext::PRIMITIVE_TRIANGLE_FAN, s_vertices, SPOTLIGHT_SEGMENTS+2);
}