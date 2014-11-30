#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <x2d/x2d.h>
#include "scene.h"

class UiObject;

class GameScene : public Scene
{
public:
	void showEvent();
	void hideEvent();
	void drawEvent();
	void updateEvent();

public:
	vector<UiObject*> m_uiObjects;
};

#endif // GAME_SCENE_H