#include "gamemanager.h"
#include "debug.h"
#include "world.h"
#include "camera.h"
#include "constants.h"
#include "blockdata.h"

#include "gui/canvas.h"
#include "gui/uiobject.h"

#include "game/camera.h"

#include "scenes.h"
#include "game/gamemanager.h"

UiObject *canvas = nullptr;

list<GameObject*> GameManager::s_gameObjects;

void GameManager::main()
{
	SceneManager::init();

	// Set some key bindings
	XInput::bind(XD_KEY_ESCAPE, function<void()>(XEngine::exit));
	//XInput::bind(XD_KEY_I, &delegate<void()>(Camera::zoomIn));
	//XInput::bind(XD_KEY_O, &delegate<void()>(Camera::zoomOut));
	//XInput::bind(XD_KEY_C, &delegate<void()>(Debug::debugFunc));
	//XInput::bind(XD_BACKSPACE, &delegate<void()>());

	BlockData::init();
	World::init();

	XWindow::setSize(Vector2i(1280, 720));

	SceneManager::gotoScene(SCENE_MAIN_MENU);//SCENE_GAME);
}

void GameManager::exit()
{
	delete canvas;
}

void GameManager::update()
{
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		(*itr)->update();
	}
}

void GameManager::draw()
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
		if(usingSceneMat)
		{
			batch.setProjectionMatrix(Matrix4());
			usingSceneMat = false;
		}

		Debug::draw(&batch);
	}

	XGraphics::renderBatch(batch);

	SceneManager::update();
}

void GameManager::addGameObject(GameObject *object)
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

void GameManager::removeGameObject(GameObject *object)
{
	s_gameObjects.remove(object);
}