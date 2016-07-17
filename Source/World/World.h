#ifndef WORLD_H
#define WORLD_H

#include "Config.h"

#include "Entities/Entity.h"

#include "Controllers/Terrain.h"
#include "Controllers/Lighting.h"
#include "Controllers/Background.h"
#include "Controllers/Camera.h"
#include "Controllers/TimeOfDay.h"

class Server;
class Client;
class BlockEntity;
class Player;
class Pawn;
class EntityLayer;

/**
* This class is concerned with the objects contained within the game world
*/
class World : public SceneObject
{
	friend class Server;
	friend class Pawn;
public:
	World(Game *game);

	void create(const string &name);
	bool load(const string &name);
	void save();
	void clear();

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	// World path
	string getWorldPath() const
	{
		return m_worldPath;
	}
	
	//IniFile *getWorldFile() const { return m_worldFile; }

	// Controller entities

	Background *getBackground() const
	{
		return m_background;
	}

	TimeOfDay *getTimeOfDay() const
	{
		return m_timeOfDay;
	}

	Camera *getCamera() const
	{
		return m_camera;
	}

	Lighting *getLighting() const
	{
		return m_lighting;
	}

	Terrain *getTerrain() const
	{
		return m_terrain;
	}

	// Entities
	void addEntity(Entity *entity);
	void removeEntity(Entity *entity);
	EntityLayer *getEntitiyLayer(const WorldLayer layer) const;
	list<Entity*> getEntities() const;

	// Pawns
	list<Pawn*> getPawns() const
	{
		return m_pawns;
	}

	// Local player
	Player *getLocalPlayer() const
	{
		return m_localPlayer;
	}

	uint getSeed() const
	{
		return m_seed;
	}

private:
	uint m_seed;

	Terrain *m_terrain;
	Background *m_background;
	TimeOfDay *m_timeOfDay;
	Camera *m_camera;
	Lighting *m_lighting;
	
	IniFile *m_worldFile;
	string m_worldPath;

	list<Pawn*> m_pawns;
	Player* m_localPlayer;

	BlockDrawer *m_blockDrawers[WORLD_LAYER_COUNT];
	EntityLayer *m_entityLayers[WORLD_LAYER_COUNT];

	list<Entity*> m_entities;
};

class EntityLayer : public SceneObject
{
public:
	EntityLayer(World *world);

	void addEntity(Entity *entity);
	void removeEntity(Entity *entity);
	void clearEntities();
	list<Entity*> getEntities() const;
	
	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

private:
	World *m_world;
	list<Entity*> m_entities;
};

#endif // WORLD_H