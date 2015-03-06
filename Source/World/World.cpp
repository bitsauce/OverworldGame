#include "World.h"
#include "Constants.h"

#include "Entities/Player.h"
#include "Entities/Background.h"
#include "Terrain/Terrain.h"
#include "Generation/Generator.h"
#include "Physics/PhysicsBody.h"
#include "Entities/Player.h"
#include "Entities/Camera.h"
#include "Entities/TimeOfDay.h"
#include "Lighting/Lighting.h"
#include "Game/Debug.h"
#include "Scenes/SceneManager.h"
#include "Scenes/GameScene.h"

string World::s_worldPath = "";
Terrain *World::s_terrain = nullptr;
WorldGenerator *World::s_generator = nullptr;
TimeOfDay *World::s_timeOfDay = nullptr;
IniFile *World::s_worldFile = nullptr;
Camera *World::s_camera = nullptr;
Debug *World::s_debug = nullptr;
Lighting *World::s_lighting = nullptr;
vector<Player*> World::s_players;
Player *World::s_localPlayer = nullptr;

void World::init()
{
	s_timeOfDay = new TimeOfDay();
	new Background(s_timeOfDay);
	s_camera = new Camera();
	s_generator = new WorldGenerator();
	s_terrain = new Terrain(s_generator, s_camera);
	s_worldFile = nullptr;
	s_lighting = new Lighting(s_terrain);
	s_debug = new Debug(s_terrain, s_lighting);
}

void World::create(const string &name)
{
	LOG("Creating world '%s'...", name.c_str());
	
	// Set the world path
	s_worldPath = "saves:/Overworld/" + name;
	s_worldFile = new IniFile(s_worldPath + "/World.ini");

	FileSystem::MakeDir(s_worldPath);
	FileSystem::MakeDir(s_worldPath + "/Chunks");
		
	// Create world file
	uint seed = Random().nextInt();
	s_worldFile->setValue("world", "name", name);
	s_worldFile->setValue("world", "seed", util::intToStr(seed));
	s_worldFile->save();

	s_generator->setSeed(seed);
}

void World::save()
{
	LOG("Saving world...");

	s_terrain->getChunkLoader()->clear();
}

bool World::load(const string &name)
{
	string worldFile = "saves:/Overworld/" + name + "/World.ini";
	if(util::fileExists(worldFile))
	{
		LOG("Loading world '%s'...", name.c_str());
	
		// Set the world path
		s_worldPath = "saves:/Overworld/" + name;
		s_worldFile = new IniFile(worldFile);
	}
	else
	{
		LOG("World '%s' not found", name.c_str());
		return false;
	}
	return true;
}

void World::clear()
{
	LOG("Reseting world...");

	s_worldPath.clear();
}

string World::getWorldPath()
{
	return s_worldPath;
}

Terrain *World::getTerrain()
{
	return s_terrain;
}

TimeOfDay *World::getTimeOfDay()
{
	return s_timeOfDay;
}

Camera *World::getCamera()
{
	return s_camera;
}

Debug *World::getDebug()
{
	return s_debug;
}

Lighting *World::getLighting()
{
	return s_lighting;
}

vector<Player*> World::getPlayers()
{
	return s_players;
}