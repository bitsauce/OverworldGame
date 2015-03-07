#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "Config.h"
#include "Scene.h"

class LineEdit;

class MultiplayerScene : public Scene
{
public:
	MultiplayerScene(World &world);

	void host();
	void join();

private:
	World &m_world;
	LineEdit *m_ipLineEdit;
	LineEdit *m_portLineEdit;
};

#endif // MULTIPLAYER_H