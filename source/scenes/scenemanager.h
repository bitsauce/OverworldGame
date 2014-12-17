#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "scene.h"

enum SceneID;

class SceneManager
{
	friend class GameManager;
public:

	// Initialize scenes
	static void init();

	// Clear scenes
	static void clear();

	// Go to scene
	static void gotoScene(const SceneID scene/*, const float time, const TransitionType transition*/);
	//static void setTransition(const float time, const TransitionType transition);

protected:

	// Update (called by GameManager)
	static void update();

private:

	// Array of scenes
	static Scene *s_scenes[];

	// Current scene
	static Scene *s_currentScene;

	// Next scene
	static Scene *s_nextScene;
};

#endif // SCENE_MANAGER_H