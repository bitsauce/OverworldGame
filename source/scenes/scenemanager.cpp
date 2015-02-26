#include "SceneManager.h"
#include "Constants.h"
#include "MainMenu.h"
#include "WorldSelect.h"
#include "WorldCreate.h"
#include "Multiplayer.h"
#include "GameScene.h"

// Init static members
Scene *SceneManager::s_scenes[SCENE_COUNT];
Scene *SceneManager::s_currentScene = nullptr;
Scene *SceneManager::s_nextScene = nullptr;

void SceneManager::init()
{
	// Init scene objects
	s_scenes[SCENE_MAIN_MENU] = new MainMenuScene;
	s_scenes[SCENE_WORLD_SELECT] = new WorldSelectScene;
	s_scenes[SCENE_WORLD_CREATE] = new WorldCreateScene;
	s_scenes[SCENE_MULTIPLAYER] = new MultiplayerScene;
	s_scenes[SCENE_GAME] = new GameScene;
}

void SceneManager::clear()
{
	// Delete scene objects
	for(uint i = 0; i < SCENE_COUNT; ++i) delete s_scenes[i]; 
}

void SceneManager::gotoScene(const SceneID scene)
{
	// Set next scene
	s_nextScene = s_scenes[scene];
}

void SceneManager::update()
{
	// Load next scene if any
	if(s_nextScene)
	{
		s_nextScene->showEvent();
		if(s_currentScene) s_currentScene->hideEvent();

		s_currentScene = s_nextScene;
		s_nextScene = nullptr;
	}
}