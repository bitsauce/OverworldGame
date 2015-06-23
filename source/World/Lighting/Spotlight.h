#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Config.h"
#include "LightSource.h"

#define SPOTLIGHT_SEGMENTS 32

class Lighting;

class Spotlight : public LightSource
{
	friend class Terrain;
public:
	Spotlight(Lighting *lighting, const Vector2 &position, const float radius, const xd::Color &color);

	// Draw spotlight
	void draw(xd::SpriteBatch *spriteBatch);

private:
	// TODO: Maybe I should just have a global array of vct vertices that can be reused whenever
	static xd::Vertex *s_vertices;
};
//>REMOVE ME<
#endif // SPOTLIGHT_H