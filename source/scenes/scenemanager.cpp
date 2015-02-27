#include "SceneManager.h"
#include "Constants.h"
#include "MainMenu.h"
#include "WorldSelect.h"
#include "WorldCreate.h"
#include "Multiplayer.h"
#include "GameScene.h"

Scene *SceneManager::s_scene = nullptr;
stack<Scene*> SceneManager::s_scenesToDelete;

void SceneManager::setScene(Scene *scene)
{
	// Queue scene for deletion
	if(s_scene)
	{
		s_scenesToDelete.push(s_scene);
	}

	// Set next scene
	s_scene = scene;
}

void SceneManager::update()
{
	// Delete scenes
	while(!s_scenesToDelete.empty())
	{
		delete s_scenesToDelete.top();
		s_scenesToDelete.pop();
	}
}