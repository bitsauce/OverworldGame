#ifndef WORLD_SELECT_H
#define WORLD_SELECT_H

#include "Config.h"
#include "Scene.h"

class GameObject;

class WorldSelectScene : public Scene
{
public:
	void showEvent();
	void hideEvent();
	void drawEvent();
	void updateEvent();

	void worldClicked();
	void showCreateWorld();

private:
	vector<GameObject*> m_sceneObjects;
};

#endif // WORLD_SELECT_H