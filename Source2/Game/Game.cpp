#include "Game.h"
//>REMOVE ME<
#include "Constants.h"
#include "Gui/Canvas.h"
#include "Gui/UiObject.h"
#include "Blocks/BlockData.h"
#include "Items/ItemData.h"
#include "Things/Thing.h"
#include "Things/ThingData.h"
#include "World/World.h"
#include "Game/Debug.h"
#include "World/Camera.h"
#include "World/Background.h"
#include "World/TimeOfDay.h"
#include "World/Lighting/Lighting.h"
#include "World/Lighting/Spotlight.h"
#include "World/Terrain/Terrain.h"
#include "Networking/Server.h"
#include "Networking/Client.h"
#include "Entities/Entity.h"
#include "GameStates/InGameState.h"

Game::Game() :
	m_spriteBatch(nullptr),
	m_world(nullptr),
	m_takeScreenshot(false),
	m_client(nullptr),
	m_server(nullptr),
	m_gameOverlay(nullptr)
{
}

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
	
	// Init world
	m_world = new World();

	// Initialize block and item data
	BlockData::init();
	ItemData::init(this);
	ThingData::init(this);

	// Setup debug
	m_debug = new Debug(this);

	// Resize the window
	Window::setSize(Vector2i(1280, 720));

	// Load world "Debug", or create it if it doesn't exists
	if(!m_world->load("Debug"))
	{
		m_world->create("Debug");
	}
	
	LOG("Hosting local server on port '45556'...");

	// Create server object
	m_server = new Server(this, 45556);

	// Join server as client
	RakNet::BitStream bitStream;
	bitStream.Write((RakNet::MessageID)ID_PLAYER_JOIN);
	bitStream.Write("Bitsauce");
	m_server->getRakPeer()->SendLoopback((const char*)bitStream.GetData(), bitStream.GetNumberOfBytesUsed());
		
	// Push game state
	pushState(new InGameState(this));
}

void Game::pushState(GameState *state)
{
	assert(state);
	m_states.push_front(state);
	state->enter();
}

void Game::popState()
{
	if(!m_states.empty())
	{
		GameState *front = m_states.front();
		front->leave();
		m_states.remove(front);
		delete front;
	}
	else
	{
		exit();
	}
}

GameState *Game::peekState(int level)
{
	list<GameState*>::iterator itr = m_states.begin();
	advance(itr, level);
	return *itr;
}

void Game::exit()
{
	// Save the world as we're exiting
	m_world->save();
	m_server->save();
	delete m_world;
}

void Game::update(const float delta)
{
	// Update game connections
	if(Connection::getInstance()->isServer())
	{
		((Server*)Connection::getInstance())->update();
	}
	else
	{
		((Client*)Connection::getInstance())->update();
	}

	// Update game states
	for(list<GameState*>::iterator itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		(*itr)->update(delta);
		if(!(*itr)->isTransparent())
		{
			break;
		}
	}

	// Update debug
	m_debug->update();
}

void Game::draw(GraphicsContext &context, const float alpha)
{
	// Take screen shot
	if(m_takeScreenshot)
	{
		int i = 0;
		while(util::fileExists("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png")) i++;
		context.saveScreenshot("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png");
		m_takeScreenshot = false;
	}
	
	// Draw game states
	for(list<GameState*>::iterator itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		(*itr)->draw(m_spriteBatch, alpha);
		if(!(*itr)->isTransparent())
		{
			break;
		}
	}
	
	// Render debug stuff
	m_debug->setVariable("FPS", util::intToStr((int)Graphics::getFPS()));
	m_spriteBatch->begin();
	m_debug->draw(m_spriteBatch);
	m_spriteBatch->end();
}