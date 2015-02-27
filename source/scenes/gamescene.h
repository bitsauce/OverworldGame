#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Config.h"
#include "Scene.h"

class GameOverlay;

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

public:
	GameOverlay *m_inventory;
};

#endif // GAME_SCENE_H