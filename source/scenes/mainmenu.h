#ifndef MAINMENU_H
#define MAINMENU_H

#include "Config.h"
#include "scene.h"

class GameObject;

class MainMenuScene : public Scene
{
public:
	MainMenuScene();

private:
	void showWorldSelectScene();
};

#endif // MAINMENU_H