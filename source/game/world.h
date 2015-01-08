#ifndef WORLD_H
#define WORLD_H

#include <x2d/x2d.h>

class Terrain;
class TimeOfDay;
class Camera;
class Debug;
class Lighting;

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
	static Debug *getDebug();
	static Lighting *getLighting();

private:
	
	static Terrain *s_terrain;
	static TimeOfDay *s_timeOfDay;
	static string s_worldPath;
	static xd::IniFile *s_worldFile;
	static Camera *s_camera;
	static Debug *s_debug;
	static Lighting *s_lighting;
};

#endif // WORLD_H