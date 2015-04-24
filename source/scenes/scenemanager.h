#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Config.h"
#include "Scene.h"

enum SceneID;

class SceneManager
{
	friend class Game;
public:
	// Go to scene
	static void setScene(Scene *scene/*, const float time, const TransitionType transition*/);
	static Scene *getScene() { return s_scene; }

protected:
	// Update (called by Game)
	static void update();

private:
	// Current scene
	static Scene *s_scene;

	// Scene to delete
	static stack<Scene*> s_scenesToDelete;
};

#endif // SCENE_MANAGER_H