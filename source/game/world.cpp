#include "world.h"
#include "constants.h"

#include "environment/timeofday.h"
#include "environment/background.h"

#include "terrain/terrain.h"

#include "game.h"

#include "player.h"
#include "physics/physicsbody.h"

string World::s_worldPath = "";
Terrain *World::s_terrain = nullptr;
TimeOfDay *World::s_timeOfDay = nullptr;
xd::IniFile *World::s_worldFile = nullptr;
Camera *World::s_camera = nullptr;
Debug *World::s_debug = nullptr;
Lighting *World::s_lighting = nullptr;

void World::init()
{
	s_timeOfDay = new TimeOfDay();
	new Background(s_timeOfDay);
	s_camera = new Camera();
	s_terrain = new Terrain();
	s_worldFile = nullptr;
	Player *p = new Player(); p->getBody()->setPosition(0, 0);
	//s_camera->lookAt(Vector2(0.0f, 0.0f));
	s_lighting = new Lighting(s_terrain);
	s_debug = new Debug(s_terrain, s_lighting);
}

void World::create(const string &name)
{
	xd::LOG("Creating world '%s'...", name.c_str());
	
	// Set the world path
	s_worldPath = "saves:/Overworld/" + name;
	s_worldFile = new xd::IniFile(s_worldPath + "/world.ini");
		
	// Create world file
	s_worldFile->setValue("world", "name", name);
	s_worldFile->save();
		
	xd::LOG("Creating player...");
		
	// Create player
	/*Player player();
	player.body.setPosition(Vector2(0, Terrain.generator.getGroundHeight(0)*BLOCK_PX));
		
	// Give default loadout
	player.inventory.addItem(@Items[ITEM_PICKAXE_IRON]);
	player.inventory.addItem(@Items[ITEM_AXE_IRON]);*/
		
	// Show game
	SceneManager::gotoScene(SCENE_GAME);
}

void World::save()
{
	xd::LOG("Saving world...");
}

void World::clear()
{
	xd::LOG("Reseting world...");

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