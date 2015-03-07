#ifndef MAINMENU_H
#define MAINMENU_H

#include "Config.h"
#include "scene.h"

class World;

class MainMenuScene : public Scene
{
public:
	MainMenuScene(World &world);

private:
	void showWorldSelectScene();

	World &m_world;
};

#endif // MAINMENU_H