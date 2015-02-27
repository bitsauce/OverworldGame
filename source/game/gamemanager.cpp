#include "GameManager.h"
#include "Constants.h"
#include "Gui/Canvas.h"
#include "Gui/UiObject.h"
#include "Lighting/Spotlight.h"
#include "World/World.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Multiplayer.h"
#include "Blocks/BlockData.h"
#include "Items/ItemData.h"
#include "Things/ThingData.h"
#include "Game/Debug.h"
#include "Entities/Camera.h"

UiObject *canvas = nullptr;

list<GameObject*> GameManager::s_gameObjects;
SpriteBatch *GameManager::s_spriteBatch = nullptr;
bool GameManager::s_takeScreenshot = false;

void GameManager::main()
{
	// Set some key bindings
	Input::bind(XD_KEY_ESCAPE, function<void()>(Engine::exit));
	Input::bind(XD_KEY_SNAPSHOT, function<void()>(GameManager::takeScreenshot));
	
	// Initialize game managers
	BlockData::init();
	ItemData::init();
	World::init();
	ThingData::init(World::getTerrain());

	// Enable debugger
	World::getDebug()->toggle();

	// Resize the window
	Window::setSize(Vector2i(1280, 720));

	// Show main menu
	//if(!World::load("Debug"))
	//{
	//	World::create("Debug");
	//}

	SceneManager::setScene(new MultiplayerScene());
}

void GameManager::exit()
{
	World::save();
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

void GameManager::draw(GraphicsContext &context)
{
	if(!s_spriteBatch) s_spriteBatch = new SpriteBatch(context);

	if(s_takeScreenshot)
	{
		int i = 0;
		while(util::fileExists("C:\\Users\\Marcus\\Desktop\\screenshot_" + util::intToStr(i) + ".png")) i++;
		context.saveScreenshot("C:\\Users\\Marcus\\Desktop\\screenshot_" + util::intToStr(i) + ".png");
		s_takeScreenshot = false;
	}
	
	World::getDebug()->setVariable("FPS", util::intToStr((int)Graphics::getFPS()));

	s_spriteBatch->begin();
	bool usingSceneMat = false;
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		if(DRAW_ORDER_SCENE_START < (*itr)->m_depth && DRAW_ORDER_SCENE_END > (*itr)->m_depth)
		{
			if(!usingSceneMat)
			{
				s_spriteBatch->end();
				s_spriteBatch->begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState::PRESET_ALPHA_BLEND, World::getCamera()->getProjectionMatrix()));
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