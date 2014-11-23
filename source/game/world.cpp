#include "world.h"
#include "constants.h"

#include "environment/timeofday.h"
#include "environment/background.h"

#include "scenes/gamescene.h"

#include "terrain/terrain.h"

#include <Box2D/Box2D.h>

b2World *World::s_b2World = nullptr;
Terrain *World::s_terrain = nullptr;
TimeOfDay *World::s_timeOfDay = nullptr;
string World::s_worldPath;
XIniFile *World::s_worldFile = nullptr;

void World::init()
{
	s_timeOfDay = new TimeOfDay();
	new Background(s_timeOfDay);
	s_b2World = new b2World(b2Vec2(0.0f, 9.81f));
	s_terrain = new Terrain;
	s_worldFile = nullptr;
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
	player.body.setPosition(Vector2(0, Terrain.generator.getGroundHeight(0)*TILE_PX));
		
	// Give default loadout
	player.inventory.addItem(@Items[ITEM_PICKAXE_IRON]);
	player.inventory.addItem(@Items[ITEM_AXE_IRON]);*/
		
	// Show game
	XEngine::pushScene(scene::game);
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

b2World *World::getb2World()
{
	return s_b2World;
}

Terrain *World::getTerrain()
{
	return s_terrain;
}