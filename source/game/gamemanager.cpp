#include "GameManager.h"
#include "Constants.h"
#include "Gui/Canvas.h"
#include "Gui/UiObject.h"
#include "Scenes/SceneManager.h"
#include "Scenes/GameScene.h"

#include "Blocks/BlockData.h"

#include "Items/ItemData.h"

#include "Things/ThingData.h"

#include "World/World.h"
#include "World/Debug.h"
#include "World/Camera.h"
#include "World/Background.h"
#include "World/TimeOfDay.h"
#include "World/Lighting/Lighting.h"
#include "World/Lighting/Spotlight.h"

#include "Networking/Server.h"
#include "Networking/Client.h"

#include "Entities/Entity.h"

#include "World/Terrain/Terrain.h"

#include "Scenes/Scene.h"

Canvas *canvas = nullptr;

SpriteBatch *GameManager::s_spriteBatch = nullptr;
bool GameManager::s_takeScreenshot = false;
World *GameManager::m_world = nullptr;

void GameManager::main(GraphicsContext &context)
{
	// Sprite batch
	s_spriteBatch = new SpriteBatch(context);

	// Set some key bindings
	Input::bind(XD_KEY_ESCAPE, function<void()>(Engine::exit));
	Input::bind(XD_KEY_SNAPSHOT, function<void()>(GameManager::takeScreenshot));
	//Window::enableFullscreen();
	
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

void GameManager::update(const float dt)
{
	if(Connection::getInstance()->isServer())
	{
		((Server*)Connection::getInstance())->update();
	}
	else
	{
		((Client*)Connection::getInstance())->update();
	}
	
	m_world->getTimeOfDay()->update(dt);
	m_world->getBackground()->update(dt);
	//m_world->getCamera()->update();
	m_world->getDebug()->update();

	m_world->getTerrain()->getChunkLoader()->update();

	list<Entity*> gameObjects = m_world->getEntities();
	for(list<Entity*>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->update(dt);
	}

	list<UiObject*> uiObjects = SceneManager::getScene()->getUiObjects();
	for(UiObject *object : uiObjects)
	{
		object->update(dt);
	}
}

void GameManager::draw(GraphicsContext &context, const float alpha)
{
	if(s_takeScreenshot)
	{
		int i = 0;
		while(util::fileExists("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png")) i++;
		context.saveScreenshot("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png");
		s_takeScreenshot = false;
	}

	s_spriteBatch->begin();
	
	m_world->getDebug()->setVariable("FPS", util::intToStr((int)Graphics::getFPS()));
	m_world->getBackground()->draw(s_spriteBatch, alpha);
	
	m_world->getCamera()->draw(s_spriteBatch, alpha);

	m_world->getTerrain()->m_background.draw(s_spriteBatch);
	m_world->getTerrain()->m_middleground.draw(s_spriteBatch);

	s_spriteBatch->end();
	s_spriteBatch->begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState::PRESET_ALPHA_BLEND, m_world->getCamera()->getProjectionMatrix()));

	list<Entity*> gameObjects = m_world->getEntities();
	for(list<Entity*>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->draw(s_spriteBatch, alpha);
	}
	
	m_world->getTerrain()->m_foreground.draw(s_spriteBatch);
	m_world->getLighting()->draw(s_spriteBatch);
	
	s_spriteBatch->end();
	s_spriteBatch->begin();

	list<UiObject*> uiObjects = SceneManager::getScene()->getUiObjects();
	for(UiObject *object : uiObjects)
	{
		object->draw(s_spriteBatch, alpha);
	}

	m_world->getDebug()->draw(s_spriteBatch);
	s_spriteBatch->end();

	SceneManager::update();
}