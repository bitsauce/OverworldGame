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

#include "lighting/spotlight.h"

UiObject *canvas = nullptr;

list<GameObject*> GameManager::s_gameObjects;
Spotlight *GameManager::s_spotlight = nullptr;

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

	SceneManager::gotoScene(SCENE_GAME);
}

void GameManager::exit()
{
	delete canvas;
}

XRandom random;
void GameManager::update()
{
	if(XInput::getKeyState(XD_KEY_L)) {
		s_spotlight = new Spotlight(Vector2(0.0f, 0.0f), 64, xd::Color(random.nextInt(255), random.nextInt(255), random.nextInt(255), 255));
	}
	if(s_spotlight) s_spotlight->m_position.set(XInput::getPosition().x, XInput::getPosition().y);
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		(*itr)->update();
	}
}

xd::SpriteBatch *GameManager::s_spriteBatch = nullptr;

void GameManager::draw(xd::GraphicsContext &context)
{
	if(!s_spriteBatch) s_spriteBatch = new xd::SpriteBatch(context);

	s_spriteBatch->begin();
	bool usingSceneMat = false;
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		if(DRAW_ORDER_SCENE_START < (*itr)->m_depth && DRAW_ORDER_SCENE_END > (*itr)->m_depth)
		{
			if(!usingSceneMat)
			{
				s_spriteBatch->end();
				s_spriteBatch->begin(xd::SpriteBatch::State(xd::SpriteBatch::DEFERRED, xd::BlendState::PRESET_ALPHA_BLEND, World::getCamera()->getProjectionMatrix()));
				usingSceneMat = true;
			}
		}
		else
		{
			if(usingSceneMat)
			{
				s_spriteBatch->end();
				s_spriteBatch->begin();
				usingSceneMat = false;
			}
		}
		(*itr)->draw(s_spriteBatch);
	}
	
	Debug::setVariable("FPS", util::intToStr((int)xd::Graphics::getFPS()));
	if(XInput::getKeyState(XD_KEY_Z))
	{
		if(usingSceneMat)
		{
			s_spriteBatch->end();
			s_spriteBatch->begin();
			usingSceneMat = false;
		}

		Debug::draw(s_spriteBatch);
	}
	s_spriteBatch->end();

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