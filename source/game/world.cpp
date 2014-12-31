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
XIniFile *World::s_worldFile = nullptr;
Camera *World::s_camera = nullptr;
Debug *World::s_debug = nullptr;

void World::init()
{
	s_timeOfDay = new TimeOfDay();
	new Background(s_timeOfDay);
	s_camera = new Camera();
	s_terrain = new Terrain();
	s_debug = new Debug(s_terrain);
	s_worldFile = nullptr;
	//Player *p = new Player(); p->m_body->setPosition(0, 0);
	s_camera->lookAt(Vector2(0.0f, 0.0f));
}

void World::create(const string &name)
{
	LOG("Creating world '%s'...", name.c_str());
	
	// Set the world path
	s_worldPath = "saves:/Overworld/" + name;
	s_worldFile = new XIniFile(s_worldPath + "/world.ini");
		
	// Create world file
	s_worldFile->setValue("world", "name", name);
	s_worldFile->save();
		
	LOG("Creating player...");
		
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
	LOG("Saving world...");
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

Camera *World::getCamera()
{
	return s_camera;
}

Debug *World::getDebug()
{
	return s_debug;
}