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
#include "Things/Thing.h"
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
	m_terrain = new Terrain(this);
	m_lighting = new Lighting(this);
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
	LOG("Saving world->..");

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
	LOG("Reseting world->..");

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

void World::update(const float delta)
{
	m_timeOfDay->update(delta);
	m_background->update(delta);
	m_terrain->getChunkLoader()->update();

	list<Entity*> gameObjects = m_entities;
	for(list<Entity*>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->update(delta);
	}
}

void World::draw(SpriteBatch *spriteBatch, const float alpha)
{
	spriteBatch->begin();

	m_background->draw(spriteBatch, alpha);
	m_camera->update(alpha);
	
	spriteBatch->end();
	spriteBatch->begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState::PRESET_ALPHA_BLEND, m_camera->getProjectionMatrix()));

	m_terrain->m_background.draw(spriteBatch);

	set<Thing*> things = m_terrain->getChunkLoader()->getActiveThings();
	for(Thing *thing : things)
	{
		thing->draw(spriteBatch, alpha);
	}

	m_terrain->m_middleground.draw(spriteBatch);

	list<Entity*> gameObjects = m_entities;
	for(list<Entity*>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->draw(spriteBatch, alpha);
	}
	
	m_terrain->m_foreground.draw(spriteBatch);
	m_lighting->draw(spriteBatch);
	
	spriteBatch->end();
}