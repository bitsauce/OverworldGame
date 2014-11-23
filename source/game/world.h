#ifndef WORLD_H
#define WORLD_H

#include <x2d/x2d.h>

class Terrain;
class b2World;

class World
{
public:
	static void init();
	static void create(const string &name);
	static void save();
	static void clear();

	static string getWorldPath();

	static b2World *getb2World();
	static Terrain *getTerrain();

private:

	static Terrain *s_terrain;
	static b2World *s_b2World;
	static string s_worldPath;
	static XIniFile *s_worldFile;
};

#endif // WORLD_H