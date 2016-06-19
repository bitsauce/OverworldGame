#include "Pointlight.h"
#include "World/World.h"

Vertex *Pointlight::s_vertices;

Pointlight::Pointlight(World *world, const Vector2F &position, const float radius, const Color &color) :
	LightSource(position, radius, color)
{
	world->getLighting()->addLightSource(this);
	//world->getTerrain()->getChunkManager()->getChunkAt(position.x / CHUNK_PXF, position.y / CHUNK_PXF, false)->m_lightSource.push_back(this);
}

void Pointlight::draw(SpriteBatch *spriteBatch)
{
	GraphicsContext *gfxContext = spriteBatch->getGraphicsContext();
	s_vertices[0].set2f(VERTEX_POSITION, m_position.x, m_position.y);
	s_vertices[0].set4ub(VERTEX_COLOR, m_color.getR(), m_color.getG(), m_color.getB(), m_color.getA());
	s_vertices[0].set2f(VERTEX_TEX_COORD, 0.0f, 1.0f);
	for(uint i = 1; i < POINTLIGHT_SEGMENTS+2; ++i)
	{
		float r = (2.0f * PI * i) / POINTLIGHT_SEGMENTS;
		s_vertices[i].set2f(VERTEX_POSITION, m_position.x + cos(r)*m_radius, m_position.y + sin(r)*m_radius);
		s_vertices[i].set4ub(VERTEX_COLOR, m_color.getR(), m_color.getG(), m_color.getB(), 0);
		s_vertices[i].set2f(VERTEX_TEX_COORD, (1 + cos(r))/2.0f, 1.0f - (1 + sin(r))/2.0f);
	}
	gfxContext->drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_FAN, s_vertices, POINTLIGHT_SEGMENTS + 2);
}