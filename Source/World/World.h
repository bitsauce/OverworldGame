#ifndef WORLD_H
#define WORLD_H

#include "Config.h"

#include "Entities/Entity.h"

#include "World/Terrain.h"
#include "Lighting/Lighting.h"
#include "Background.h"
#include "Camera.h"
#include "TimeOfDay.h"

class WorldGenerator;
class Server;
class Client;
class BlockEntity;
class Background;
class Player;
class Pawn;

/**
* This class is concerned with the objects contained within the game world
*/
class World : public GameObject
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

	ChunkManager *getChunkManager() const
	{
		return m_chunkManager;
	}

	// Entities
	void addEntity(Entity *entity);
	void removeEntity(Entity *entity);
	list<Entity*> getEntities() const;
	list<Entity*> getEntitiesByLayer(const WorldLayer layer) const;

	// Pawns
	/*list<Pawn*> getPawns() const
	{
		return m_pawns;
	}

	// Local player
	Player *getLocalPlayer() const
	{
		return m_localPlayer;
	}*/

private:
	Terrain *m_terrain;
	Background *m_background;
	TimeOfDay *m_timeOfDay;
	Camera *m_camera;
	Lighting *m_lighting;
	
	IniFile *m_worldFile;
	string m_worldPath;

	list<Pawn*> m_pawns;
	Player* m_localPlayer;

	// Entities
	vector<list<Entity*>> m_entitiesByLayer;
	list<Entity*> m_entities;

	// Chunk manager
	ChunkManager *m_chunkManager; // ChunkGenerator is owned by ChunkManager
};

#endif // WORLD_H