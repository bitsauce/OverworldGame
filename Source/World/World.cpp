#include "World.h"
#include "Constants.h"

#include "Entities/EntityData.h"
#include "Entities/DynamicEntity.h"
#include "Entities/Pawn.h"

#include "BlockEntities/BlockEntity.h"

#include "Game/Debug.h"
#include "Generation/Generator.h"

World::World(Game *game) :
	m_worldPath(""),
	m_worldFile(nullptr)
{
	Pointlight::s_vertices = new Vertex[POINTLIGHT_SEGMENTS + 2];

	// Load world content
	m_timeOfDay = new TimeOfDay(this);
	addChildLast(m_timeOfDay);

	m_camera = new Camera(this, game->getInputManager(), game->getWindow());
	addChildLast(m_camera);

	m_background = new Background(this, game->getWindow());
	addChildLast(m_background);

	m_lighting = new Lighting(this);
	m_terrain = new Terrain(this, game->getWindow());
	addChildLast(m_terrain);

	for(uint i = 0; i < WORLD_LAYER_COUNT; ++i)
	{
		m_blockDrawers[i] = new BlockDrawer(this, (WorldLayer) i);
		addChildLast(m_blockDrawers[i]);

		m_entityLayers[i] = new EntityLayer(this);
		addChildLast(m_entityLayers[i]);
	}

	addChildLast(m_lighting);
}

void World::create(const string &name)
{
	LOG("Creating world '%s'...", name.c_str());

	// Set the world path
	m_worldPath = "prefs:/" + name;
	m_worldFile = new IniFile(m_worldPath + "/World.ini");

	FileSystem::MakeDir(m_worldPath);
	FileSystem::MakeDir(m_worldPath + "/Chunks");
	FileSystem::MakeDir(m_worldPath + "/Players");
	FileSystem::MakeDir(m_worldPath + "/Entities");

	// Create world file
	m_seed = Random().nextInt();
	m_worldFile->setValue("world", "name", name);
	m_worldFile->setValue("world", "seed", util::intToStr(m_seed));
	m_worldFile->save();

	//m_generator->setSeed(seed);
}

void World::save()
{
}

bool World::load(const string &name)
{
	string worldFile = "prefs:/" + name + "/World.ini";
	if(util::fileExists(worldFile))
	{
		LOG("Loading world '%s'...", name.c_str());

		// Set the world path
		m_worldPath = "prefs:/" + name;
		m_worldFile = new IniFile(worldFile);

		m_seed = util::strToInt(m_worldFile->getValue("world", "seed"));

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
	LOG("Resetting world...");

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
		m_entityLayers[i]->clearEntities();
	}

	m_pawns.clear();
	m_localPlayer = nullptr;
}

void World::onTick(TickEvent *e)
{
	SceneObject::onTick(e);
}

void World::onDraw(DrawEvent *e)
{
	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	spriteBatch->flush();
	SceneObject::onDraw(e);
}

void World::addEntity(Entity *entity)
{
	m_entityLayers[entity->getData()->getLayer()]->addEntity(entity);
	m_entities.push_back(entity);
}

void World::removeEntity(Entity *entity)
{
	m_entityLayers[entity->getData()->getLayer()]->removeEntity(entity);
	m_entities.remove(entity);
}

list<Entity*> World::getEntities() const
{
	return m_entities;
}

EntityLayer *World::getEntitiyLayer(const WorldLayer layer) const
{
	return m_entityLayers[layer];
}

EntityLayer::EntityLayer(World *world) :
	m_world(world)
{
}

void EntityLayer::addEntity(Entity *entity)
{
	m_entities.push_back(entity);
	addChildLast(entity);
}

void EntityLayer::removeEntity(Entity * entity)
{
	m_entities.remove(entity);
	removeChild(entity);
}

void EntityLayer::clearEntities()
{
	m_entities.clear();
}

list<Entity*> EntityLayer::getEntities() const
{
	return m_entities;
}

void EntityLayer::onTick(TickEvent *e)
{
	SceneObject::onTick(e);
}

void EntityLayer::onDraw(DrawEvent *e)
{
	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	
	spriteBatch->end();
	
	SpriteBatch::State state;
	state.transformationMatix = m_world->getCamera()->getTransformationMatrix(e->getAlpha());

	spriteBatch->begin(state);

	SceneObject::onDraw(e);
}
