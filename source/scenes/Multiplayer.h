#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "Config.h"
#include "scene.h"

class GameObject;

class MultiplayerScene : public Scene
{
public:
	void showEvent();
	void hideEvent();
	void drawEvent();
	void updateEvent();

	void host() { LOG("HOST"); }
	void join() { LOG("JOIN"); }

private:
	vector<GameObject*> m_sceneObjects;
};

#endif // MULTIPLAYER_H