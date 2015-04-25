#ifndef MAINMENU_H
#define MAINMENU_H

#include "Config.h"
#include "scene.h"

class World;
class Scene;

class MainMenuScene : public Scene
{
public:
	MainMenuScene(Scene *scene, World *world);

private:
	void showWorldSelectScene();

	World *m_world;
};

#endif // MAINMENU_H