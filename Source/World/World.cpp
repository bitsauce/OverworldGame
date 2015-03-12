#include "World.h"
#include "Constants.h"

#include "Entities/Player.h"

#include "World/Background.h"
#include "World/Camera.h"
#include "World/TimeOfDay.h"
#include "World/Debug.h"

#include "Terrain/Terrain.h"

#include "Generation/Generator.h"
#include "Entities/Physics/DynamicEntity.h"
#include "Entities/Player.h"
#include "Lighting/Lighting.h"
#include "Scenes/SceneManager.h"
#include "Scenes/GameScene.h"

World::World() :
	m_worldPath(""),
	m_worldFile(nullptr)
{
	// Load game managers
	m_timeOfDay = new TimeOfDay();
	m_background = new Background(m_timeOfDay);
	m_camera = new Camera();
	m_generator = new WorldGenerator();
	m_terrain = new Terrain(*this);
	m_lighting = new Lighting(*this);
	m_debug = new Debug(*this);
}

void World::create(const string &name)
{
	LOG("Creating world '%s'...", name.c_str());
	
	// Set the world path
	m_worldPath = "saves:/Overworld/" + name;
	m_worldFile = new IniFile(m_worldPath + "/World.ini");

	FileSystem::MakeDir(m_worldPath);
	FileSystem::MakeDir(m_worldPath + "/Chunks");
		
	// Create world file
	uint seed = Random().nextInt();
	m_worldFile->setValue("world", "name", name);
	m_worldFile->setValue("world", "seed", util::intToStr(seed));
	m_worldFile->save();

	m_generator->setSeed(seed);
}

void World::save()
{
	LOG("Saving world...");

	m_terrain->getChunkLoader()->clear();
}

bool World::load(const string &name)
{
	string worldFile = "saves:/Overworld/" + name + "/World.ini";
	if(util::fileExists(worldFile))
	{
		LOG("Loading world '%s'...", name.c_str());
	
		// Set the world path
		m_worldPath = "saves:/Overworld/" + name;
		m_worldFile = new IniFile(worldFile);
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

	// Reset world path and file
	m_worldPath.clear();
	m_worldFile->save();
	delete m_worldFile;
	m_worldFile = nullptr;

	// Delete all entities
	for(Entity *entity : m_entities)
	{
		delete entity;
	}
	m_entities.clear();
	m_players.clear();
	m_localPlayer = nullptr;
}