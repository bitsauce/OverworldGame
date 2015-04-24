#ifndef WORLD_H
#define WORLD_H

#include "Config.h"

class Terrain;
class TimeOfDay;
class Camera;
class Debug;
class Lighting;
class WorldGenerator;
class Server;
class Client;
class Entity;
class Background;

// This class is concerned with the objects contained within the world
class World
{
	friend class Player;
public:
	World();
	void create(const string &name);
	bool load(const string &name);
	void save();
	void clear();

	// World path
	string getWorldPath() const { return m_worldPath; }
	IniFile *getWorldFile() const { return m_worldFile; }

	// Game managers
	Terrain *getTerrain() const { return m_terrain; }
	Background *getBackground() const { return m_background; }
	TimeOfDay *getTimeOfDay() const { return m_timeOfDay; }
	Camera *getCamera() const { return m_camera; }
	Lighting *getLighting() const { return m_lighting; }
	Debug *getDebug() const { return m_debug; }
	WorldGenerator *getGenerator() const { return m_generator; }

	// Entities
	void addEntity(Entity *entity) { m_entities.push_back(entity); }
	void removeEntity(Entity *entity) { m_entities.remove(entity); }
	list<Entity*> getEntities() const { return m_entities; }
	list<Player*> getPlayers() const { return m_players; }
	Player *getLocalPlayer() const { return m_localPlayer; }

private:
	Terrain *m_terrain;
	Background *m_background;

	WorldGenerator *m_generator;
	TimeOfDay *m_timeOfDay;
	string m_worldPath;
	IniFile *m_worldFile;
	Camera *m_camera;
	Debug *m_debug;
	Lighting *m_lighting;
	list<Player*> m_players;
	Player* m_localPlayer;
	Server *m_server;
	Client *m_client;
	list<Entity*> m_entities;
};

#endif // WORLD_H