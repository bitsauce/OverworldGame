#include "GameManager.h"
#include "Constants.h"
#include "Gui/Canvas.h"
#include "Gui/UiObject.h"
#include "Lighting/Spotlight.h"
#include "World/World.h"
#include "Scenes/SceneManager.h"
#include "Scenes/GameScene.h"
#include "Blocks/BlockData.h"
#include "Items/ItemData.h"
#include "Things/ThingData.h"
#include "Game/Debug.h"
#include "Entities/Camera.h"
#include "Networking/Server.h"

UiObject *canvas = nullptr;

list<GameObject*> GameManager::s_gameObjects;
SpriteBatch *GameManager::s_spriteBatch = nullptr;
bool GameManager::s_takeScreenshot = false;
World *GameManager::m_world = nullptr;

void GameManager::main()
{
	// Set some key bindings
	Input::bind(XD_KEY_ESCAPE, function<void()>(Engine::exit));
	Input::bind(XD_KEY_SNAPSHOT, function<void()>(GameManager::takeScreenshot));
	
	// Initialize game managers
	BlockData::init();
	ItemData::init();
	m_world = new World();
	ThingData::init(m_world);

	// Resize the window
	Window::setSize(Vector2i(1280, 720));

	// Show main menu
	if(!m_world->load("Debug"))
	{
		m_world->create("Debug");
	}
	
	new Server(*m_world, 5555);
		
	// Show game
	SceneManager::setScene(new GameScene(*m_world));
}

void GameManager::exit()
{
	m_world->save();
	delete m_world;
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
		while(util::fileExists("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png")) i++;
		context.saveScreenshot("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png");
		s_takeScreenshot = false;
	}
	
	m_world->getDebug()->setVariable("FPS", util::intToStr((int)Graphics::getFPS()));

	s_spriteBatch->begin();
	bool usingSceneMat = false;
	for(list<GameObject*>::iterator itr = s_gameObjects.begin(); itr != s_gameObjects.end(); ++itr)
	{
		if(PRIORITY_SCENE_START < (*itr)->m_depth && PRIORITY_SCENE_END > (*itr)->m_depth)
		{
			if(!usingSceneMat)
			{
				s_spriteBatch->end();
				s_spriteBatch->begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState::PRESET_ALPHA_BLEND, m_world->getCamera()->getProjectionMatrix()));
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