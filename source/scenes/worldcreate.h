#ifndef WORLD_CREATE_H
#define WORLD_CREATE_H

#include "Config.h"
#include "Scene.h"

class LineEdit;
class World;

class WorldCreateScene : public Scene
{
public:
	WorldCreateScene(World &world);

	void showWorldSelectScene();
	void createWorld();

private:
	World &m_world;
	LineEdit *m_worldNameEdit;
};

#endif // WORLD_CREATE_H