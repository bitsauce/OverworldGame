#ifndef WORLD_H
#define WORLD_H

#include <x2d/x2d.h>

class b2World;
class Terrain;
class TimeOfDay;

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
	static TimeOfDay *getTimeOfDay();

private:
	
	static b2World *s_b2World;
	static Terrain *s_terrain;
	static TimeOfDay *s_timeOfDay;
	static string s_worldPath;
	static XIniFile *s_worldFile;
};

#endif // WORLD_H