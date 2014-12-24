#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <x2d/x2d.h>

#define SPOTLIGHT_SEGMENTS 32

class Spotlight
{
public:
	Spotlight(const Vector2 &position, const float radius, const xd::Color &color);

	void draw(xd::GraphicsContext &gfxContext);
	static void drawAll(xd::GraphicsContext &gfxContext);

//private:

	Vector2 m_position;
	float m_radius;
	xd::Color m_color;


	static xd::Vertex *s_vertices;
	static list<Spotlight*> s_spotLights;
};

#endif // SPOTLIGHT_H