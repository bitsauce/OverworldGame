#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Config.h"
#include "LightSource.h"

#define POINTLIGHT_SEGMENTS 32

class Lighting;

class Pointlight : public LightSource
{
	friend class Terrain;
public:
	Pointlight(Lighting *lighting, const Vector2 &position, const float radius, const Color &color);

	// Draw spotlight
	void draw(SpriteBatch *spriteBatch);

private:
	// TODO: Maybe I should just have a global array of vct vertices that can be reused whenever
	static Vertex *s_vertices;
};

#endif // SPOTLIGHT_H