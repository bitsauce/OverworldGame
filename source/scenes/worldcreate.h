#ifndef WORLD_CREATE_H
#define WORLD_CREATE_H

#include "Config.h"
#include "Scene.h"

class GameObject;
class LineEdit;

class WorldCreateScene : public Scene
{
public:
	WorldCreateScene();

	void showWorldSelectScene();
	void createWorld();

private:
	LineEdit *m_worldNameEdit;
};

#endif // WORLD_CREATE_H