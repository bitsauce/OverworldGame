#ifndef WORLD_SELECT_H
#define WORLD_SELECT_H

#include <x2d/x2d.h>
#include "scene.h"

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