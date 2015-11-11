#include "World.h"
#include "Constants.h"

#include "World/Background.h"
#include "World/Camera.h"
#include "World/TimeOfDay.h"

#include "Entities/EntityData.h"
#include "Entities/Dynamic/Pawn.h"

#include "BlockEntities/BlockEntity.h"

#include "Game/Debug.h"
#include "Terrain/Terrain.h"
#include "Generation/Generator.h"
#include "Entities/Dynamic/DynamicEntity.h"
#include "Entities/Dynamic/Pawn.h"
#include "Lighting/Lighting.h"

World::World() :
	m_worldPath(""),
	m_worldFile(nullptr),
	m_entitiesByLayer(WORLD_LAYER_COUNT)
{
	// Load world content
	m_timeOfDay = new TimeOfDay();
	m_camera = new Camera();
	m_background = new Background(this);
	m_generator = new WorldGenerator(9823);
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
	FileSystem::MakeDir(m_worldPath + "/Players");
	FileSystem::MakeDir(m_worldPath + "/Entities");
		
	// Create world file
	uint seed = Random().nextInt();
	m_worldFile->setValue("world", "name", name);
	m_worldFile->setValue("world", "seed", util::intToStr(seed));
	m_worldFile->save();

	//m_generator->setSeed(seed);
}

void World::save()
{
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

		/*FileSystemIterator itr(m_worldPath + "/Objects", "*.obj", FileSystemIterator::FILES);
		while(itr.hasNext())
		{
			FileReader file(itr.next());

			int id;
			file >> id;

			switch(id)
			{
			case ENTITY_ITEM_DROP: new ItemDrop(this, file); break;
			}
		}*/
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

	for(uint i = 0; i < WORLD_LAYER_COUNT; ++i)
	{
		m_entitiesByLayer[i].clear();
	}

	m_pawns.clear();
	m_localPlayer = nullptr;
}

void World::update(const float delta)
{
	m_timeOfDay->update(delta);
	m_background->update(delta);
	m_terrain->getChunkLoader()->update(delta);
	m_camera->update(delta);
	
	list<Entity*> entities = m_entities; // We make a copy of the list so objects which are removed don't crash the iteration
	for(Entity *entity : entities)
	{
		entity->update(delta);
	}

	//list<BlockEntity*> staticEntities = ; // We make a copy of the list so objects which are removed don't crash the iteration
	for(BlockEntity *staticEntity : m_staticEntities)
	{
		staticEntity->update(delta);
	}
}

void World::draw(SpriteBatch *spriteBatch, const float alpha)
{
	spriteBatch->begin();
	
	m_camera->interpolate(alpha);

	m_terrain->getChunkLoader()->draw(spriteBatch->getGraphicsContext(), alpha);

	m_background->draw(spriteBatch, alpha);
	
	spriteBatch->end();
	spriteBatch->begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState::PRESET_ALPHA_BLEND, m_camera->getModelViewMatrix(alpha)));

	// Draw background
	m_terrain->m_background.draw(spriteBatch, alpha);
	for(Entity *entity : m_entitiesByLayer[WORLD_LAYER_BACK])
	{
		entity->draw(spriteBatch, alpha);
	}

	// Draw middleground
	m_terrain->m_middleground.draw(spriteBatch, alpha);
	for(Entity *entity : m_entitiesByLayer[WORLD_LAYER_MIDDLE])
	{
		entity->draw(spriteBatch, alpha);
	}

	for(BlockEntity *staticEntity : m_staticEntities)
	{
		staticEntity->draw(spriteBatch, alpha);
	}
	
	// Draw foreground
	m_terrain->m_foreground.draw(spriteBatch, alpha);
	for(Entity *entity : m_entitiesByLayer[WORLD_LAYER_FRONT])
	{
		entity->draw(spriteBatch, alpha);
	}

	// Draw lighting
	m_lighting->draw(spriteBatch, alpha);

	spriteBatch->end();
}

void World::addEntity(Entity *entity)
{
	m_entitiesByLayer[entity->getData()->getLayer()].push_back(entity);
	m_entities.push_back(entity);
}

void World::removeEntity(Entity *entity)
{
	m_entitiesByLayer[entity->getData()->getLayer()].remove(entity);
	m_entities.remove(entity);
}

list<Entity*> World::getEntities() const
{
	return m_entities;
}

list<Entity*> World::getEntitiesByLayer(const WorldLayer layer) const
{
	return m_entitiesByLayer[layer];
}

void World::addStaticEntity(BlockEntity *entity)
{
	m_staticEntities.push_back(entity);
}

void World::removeStaticEntity(BlockEntity *entity)
{
	m_staticEntities.remove(entity);
}