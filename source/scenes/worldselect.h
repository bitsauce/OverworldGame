#ifndef WORLD_SELECT_H
#define WORLD_SELECT_H

#include "Config.h"
#include "Scene.h"

class GameObject;
class Canvas;
class World;

class WorldSelectScene : public Scene
{
public:
	WorldSelectScene(World &world);

	void worldClicked();
	void showCreateWorld();

private:
	World &m_world;
};

#endif // WORLD_SELECT_H