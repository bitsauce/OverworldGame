#include "gamemanager.h"
#include "game.h"

#include "gui/canvas.h"
#include "gui/uiobject.h"

#include "lighting/spotlight.h"

UiObject *canvas = nullptr;

list<GameObject*> GameManager::s_gameObjects;
xd::SpriteBatch *GameManager::s_spriteBatch = nullptr;
bool GameManager::s_takeScreenshot = false;

void GameManager::main()
{
	// Set some key bindings
	xd::Input::bind(xd::XD_KEY_ESCAPE, function<void()>(xd::Engine::exit));
	xd::Input::bind(xd::XD_KEY_SNAPSHOT, function<void()>(GameManager::takeScreenshot));
	
	// Initialize game managers
	SceneManager::init();
	BlockData::init();
	ItemData::init();
	World::init();
	BlockEntityData::init(World::getTerrain());

	// Enable debugger
	World::getDebug()->toggle();

	// Resize the window
	xd::Window::setSize(Vector2i(1280, 720));

	// Show main menu
	SceneManager::gotoScene(SCENE_GAME);
}

void GameManager::exit()
{
	delete canvas;
}

void GameManager::update()
{
	list<GameObject*> gameObjects(s_gameObjects);
	for(list<GameObject*>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->update();
	}
}

void GameManager::draw(xd::GraphicsContext &context)
{
	if(!s_spriteBatch) s_spriteBatch = new xd::SpriteBatch(context);

	if(s_takeScreenshot)
	{
		int i = 0;
		while(xd::util::fileExists("C:\\Users\\Marcus\\Desktop\\screenshot_" + xd::util::intToStr(i) + ".png")) i++;
		context.saveScreenshot("C:\\Users\\Marcus\\Desktop\\screenshot_" + xd::util::intToStr(i) + ".png");
		s_takeScreenshot = false;
	}
	
	World::getDebug()->setVariable("FPS", xd::util::intToStr((int)xd::Graphics::getFPS()));

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