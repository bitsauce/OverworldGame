#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "Config.h"
#include "Game/Scene.h"
#include "Game/GameStates/GameState.h"

class LineEdit;

class MultiplayerScene : public GameState
{
public:
	MultiplayerScene(Scene *scene, World *world);

	void host();
	void join();

private:
	World *m_world;
	LineEdit *m_ipLineEdit;
	LineEdit *m_portLineEdit;
};

#endif // MULTIPLAYER_H