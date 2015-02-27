#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "Config.h"
#include "Scene.h"

class GameObject;

class MultiplayerScene : public Scene
{
public:
	MultiplayerScene();

	void host() { LOG("HOST"); }
	void join() { LOG("JOIN"); }

private:
};

#endif // MULTIPLAYER_H