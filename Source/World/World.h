#ifndef WORLD_H
#define WORLD_H

#include "Config.h"

class Terrain;
class TimeOfDay;
class Camera;
class Debug;
class Lighting;
class WorldGenerator;

class World
{
	friend class Player;
public:
	static void init();
	static void create(const string &name);
	static bool load(const string &name);
	static void save();
	static void clear();

	static string getWorldPath();

	static Terrain *getTerrain();
	static TimeOfDay *getTimeOfDay();
	static Camera *getCamera();
	static Debug *getDebug();
	static Lighting *getLighting();
	static vector<Player*> getPlayers();

private:
	static Terrain *s_terrain;
	static WorldGenerator *s_generator;
	static TimeOfDay *s_timeOfDay;
	static string s_worldPath;
	static IniFile *s_worldFile;
	static Camera *s_camera;
	static Debug *s_debug;
	static Lighting *s_lighting;
	static vector<Player*> s_players;
};

#endif // WORLD_H