#pragma once

#include "Config.h"
#include "LightSource.h"

#define POINTLIGHT_SEGMENTS 32

class Lighting;

class Pointlight : public LightSource
{
	friend class World;
public:
	Pointlight(World *world, Type type, const Vector2F &position, const float radius, const Color &color);
private:
	// TODO: Maybe I should just have a global array of vct vertices that can be reused whenever
	static Vertex *s_vertices;
};