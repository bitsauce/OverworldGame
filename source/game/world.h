#ifndef WORLD_H
#define WORLD_H

#include <x2d/x2d.h>

class b2World;
class Terrain;
class TimeOfDay;
class Camera;

class World
{
public:
	static void init();
	static void create(const string &name);
	static void save();
	static void clear();

	static string getWorldPath();

	static Terrain *getTerrain();
	static TimeOfDay *getTimeOfDay();
	static Camera *getCamera();

private:
	
	static Terrain *s_terrain;
	static TimeOfDay *s_timeOfDay;
	static string s_worldPath;
	static XIniFile *s_worldFile;
	static Camera *s_camera;
};

#endif // WORLD_H