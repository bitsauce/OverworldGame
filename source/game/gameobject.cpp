#include "gameobject.h"
#include "debug.h"
#include "world.h"
#include "camera.h"
#include "constants.h"

GameObject::GameObject(DrawOrder depth) :
	m_depth(depth)
{
	Game::addGameObject(this);
}

GameObject::~GameObject()
{
	Game::removeGameObject(this);
}

#include "scenes/mainmenu.h"
#include "scenes/worldselect.h"
#include "scenes/worldcreate.h"
#include "scenes/gamescene.h"

list<GameObject*> Game::s_gameObjects;
Scene* Game::s_scenes[SCENE_COUNT];
Scene* Game::s_currentScene = nullptr;

void Game::init()
{
	s_scenes[SCENE_MAIN_MENU] = new MainMenuScene;
	s_scenes[SCENE_WORLD_SELECT] = new WorldSelectScene;
	s_scenes[SCENE_WORLD_CREATE] = new WorldCreateScene;
	s_scenes[SCENE_GAME] = new GameScene;
}

void Game::destroy()
{
	for(uint i = 0; i < SCENE_COUNT; ++i)
	{
		delete s_scenes[i];
	}
}

void Game::gotoScene(const SceneID scene)
{
	s_currentScene = s_scenes[scene];
}

void Game::update()
{
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		(*itr)->update();
	}

	/*
	// Step Box2D
	Box2D.step(Graphics.dt);
	
	// Update all managers
	Terrain.update();
	TimeOfDay.update();
	Background.update();
	Spawner.update();
	//Water.update();
	World.update();*/
}

void Game::draw()
{
	XBatch batch;
	bool usingSceneMat = false;
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		if(DRAW_ORDER_SCENE_START < (*itr)->m_depth && DRAW_ORDER_SCENE_END > (*itr)->m_depth)
		{
			if(!usingSceneMat)
			{
				batch.setProjectionMatrix(World::getCamera()->getProjectionMatrix());
				usingSceneMat = true;
			}
		}
		else
		{
			if(usingSceneMat)
			{
				batch.setProjectionMatrix(Matrix4());
				usingSceneMat = false;
			}
		}
		(*itr)->draw(&batch);
	}
	
	Debug::setVariable("FPS", util::intToStr((int)XGraphics::getFPS()));
	if(XInput::getKeyState(XD_KEY_Z))
	{
		Debug::draw(&batch);
	}

	XGraphics::renderBatch(batch);
}

void Game::addGameObject(GameObject *object)
{
	list<GameObject*>::iterator itr;
	for(itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		if((*itr)->m_depth < object->m_depth)
			continue;
		break;
	}
	s_gameObjects.insert(itr, object);
}

void Game::removeGameObject(GameObject *object)
{
	s_gameObjects.remove(object);
}