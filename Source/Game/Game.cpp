#include "Game.h"

#include "Constants.h"
#include "Gui/Canvas.h"
#include "Gui/UiObject.h"
#include "Scenes/GameScene.h"
#include "Blocks/BlockData.h"
#include "Items/ItemData.h"
#include "Things/Thing.h"
#include "Things/ThingData.h"
#include "World/World.h"
#include "World/Debug.h"
#include "World/Camera.h"
#include "World/Background.h"
#include "World/TimeOfDay.h"
#include "World/Lighting/Lighting.h"
#include "World/Lighting/Spotlight.h"
#include "World/Terrain/Terrain.h"
#include "Networking/Server.h"
#include "Networking/Client.h"
#include "Entities/Entity.h"
#include "Scenes/Scene.h"

Canvas *canvas = nullptr;

Game::Game() :
	m_spriteBatch(nullptr),
	m_world(nullptr),
	m_takeScreenshot(false)
{
}

class InGameState : public GameState
{
};

void Game::main(GraphicsContext &context)
{
	// Setup sprite batch
	m_spriteBatch = new SpriteBatch(context);

	// Set some key bindings
	Input::bind(XD_KEY_ESCAPE, function<void()>(Engine::exit));
	Input::bind(XD_KEY_SNAPSHOT, bind(&Game::takeScreenshot, this));

	//if(Config::isFullscreenEnabled())
	//{
	//	Window::enableFullscreen();
	//}
	
	// Initialize block and item data
	BlockData::init();
	ItemData::init();

	m_world = new World();
	ThingData::init(m_world);

	// Resize the window
	Window::setSize(Vector2i(1280, 720));

	// Load world "Debug", or create it if it doesn't exists
	if(!m_world->load("Debug"))
	{
		m_world->create("Debug");
	}
	
	// Create server object
	new Server(*m_world, 45556);
		
	// Push game state
	pushState(new InGameState());
}

void Game::pushState(GameState *state)
{
	assert(state);
	m_states.push(state);
}

void Game::popState()
{
	if(!m_states.empty())
	{
		delete m_states.top();
		m_states.pop();
	}
	else
	{
		exit();
	}
}

void Game::exit()
{
	// Save the world as we're exiting
	m_world->save();
	delete m_world;
	delete canvas;
}

void Game::update(const float dt)
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

void Game::draw(GraphicsContext &context, const float alpha)
{
	if(m_takeScreenshot)
	{
		int i = 0;
		while(util::fileExists("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png")) i++;
		context.saveScreenshot("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png");
		m_takeScreenshot = false;
	}

	m_spriteBatch->begin();
	
	m_world->getDebug()->setVariable("FPS", util::intToStr((int)Graphics::getFPS()));
	m_world->getBackground()->draw(m_spriteBatch, alpha);
	
	m_world->getCamera()->update(alpha);
	
	m_spriteBatch->end();
	m_spriteBatch->begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState::PRESET_ALPHA_BLEND, m_world->getCamera()->getProjectionMatrix()));

	m_world->getTerrain()->m_background.draw(m_spriteBatch);

	set<Thing*> things = m_world->getTerrain()->getChunkLoader()->getActiveThings();
	for(Thing *thing : things)
	{
		thing->draw(m_spriteBatch, alpha);
	}

	m_world->getTerrain()->m_middleground.draw(m_spriteBatch);

	list<Entity*> gameObjects = m_world->getEntities();
	for(list<Entity*>::iterator itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->draw(m_spriteBatch, alpha);
	}
	
	m_world->getTerrain()->m_foreground.draw(m_spriteBatch);
	m_world->getLighting()->draw(m_spriteBatch);
	
	m_spriteBatch->end();
	m_spriteBatch->begin();

	list<UiObject*> uiObjects = SceneManager::getScene()->getUiObjects();
	for(UiObject *object : uiObjects)
	{
		object->draw(m_spriteBatch, alpha);
	}

	m_world->getDebug()->draw(m_spriteBatch);
	m_spriteBatch->end();

	m_states.top()->draw();
}