#include "Pointlight.h"
#include "World/World.h"

Vertex *Pointlight::s_vertices;

Pointlight::Pointlight(World *world, Type type, const Vector2F &position, const float radius, const Color &color) :
	LightSource(world, type, position, radius, color)
{
	/*int x0 = math::floor((position.x - radius) / CHUNK_BLOCKSF), y0 = math::floor((position.y - radius) / CHUNK_BLOCKSF),
		x1 = math::floor((position.x + radius) / CHUNK_BLOCKSF), y1 = math::floor((position.y + radius) / CHUNK_BLOCKSF);
	for(int y = y0; y <= y1; y++)
	{
		for(int x = x0; x <= x1; x++)
		{
			Chunk *chunk = world->getTerrain()->getChunkManager()->getChunkAt(x, y, true);
			chunk->m_attached = false;
			//chunk->m_lightSources.push_back(this);
		}
	}*/
}

/*void Pointlight::draw(SpriteBatch *spriteBatch)
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
}*/