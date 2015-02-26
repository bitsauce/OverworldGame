#ifndef MAINMENU_H
#define MAINMENU_H

#include "Config.h"
#include "scene.h"

class GameObject;

class MainMenuScene : public Scene
{
public:
	void showEvent();
	void hideEvent();
	void drawEvent();
	void updateEvent();

private:
	void showWorldSelect();

	vector<GameObject*> m_sceneObjects;
};

#endif // MAINMENU_H