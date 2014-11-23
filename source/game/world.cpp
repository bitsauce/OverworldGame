#include "world.h"

Terrain *World::s_terrain = nullptr;
b2World *World::s_b2World = nullptr;

void World::save()
{
}

void World::clear()
{
}

string World::getWorldPath()
{
	return "";
}

b2World *World::getb2World()
{
	return s_b2World;
}

Terrain *World::getTerrain()
{
	return s_terrain;
}