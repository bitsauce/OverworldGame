#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Config.h"
#include "scene.h"

class GameOverlay;

class GameScene : public Scene
{
public:
	void showEvent();
	void hideEvent();
	void drawEvent();
	void updateEvent();

public:
	GameOverlay *m_inventory;
};

#endif // GAME_SCENE_H