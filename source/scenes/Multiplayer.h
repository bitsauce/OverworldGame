#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "Config.h"
#include "Scene.h"

class GameObject;
class LineEdit;

class MultiplayerScene : public Scene
{
public:
	MultiplayerScene();

	void host();
	void join();

private:
	LineEdit *m_ipLineEdit;
	LineEdit *m_portLineEdit;
};

#endif // MULTIPLAYER_H