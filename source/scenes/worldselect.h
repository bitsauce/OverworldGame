#ifndef WORLD_SELECT_H
#define WORLD_SELECT_H

#include "Config.h"
#include "Scene.h"

class GameObject;
class Canvas;

class WorldSelectScene : public Scene
{
public:
	WorldSelectScene();

	void worldClicked();
	void showCreateWorld();
};

#endif // WORLD_SELECT_H