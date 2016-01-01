#include "Game.h"

#include "Constants.h"
#include "Gui/Canvas.h"
#include "Gui/UiObject.h"
#include "Blocks/BlockData.h"
#include "Items/ItemData.h"
#include "World/World.h"
#include "Game/Debug.h"
#include "World/Camera.h"
#include "World/Background.h"
#include "World/TimeOfDay.h"
#include "World/Lighting/Lighting.h"
#include "World/Lighting/Pointlight.h"
#include "World/Terrain/Terrain.h"
#include "Networking/Server.h"
#include "Networking/Client.h"
#include "Entities/Entity.h"
#include "Entities/EntityData.h"
#include "BlockEntities/BlockEntityData.h"
#include "GameStates/InGameState.h"
#include "Gui/GameOverlay/GameOverlay.h"

OverworldGame::OverworldGame() :
	Game("Overworld"),
	m_world(nullptr),
	m_takeScreenshot(false),
	m_client(nullptr),
	m_server(nullptr),
	m_gameOverlay(nullptr),
	m_debug(nullptr),
	m_commander(nullptr)
{
}

void OverworldGame::onStart(GameEvent *e)
{
	GraphicsContext *graphicsContext = getWindow()->getGraphicsContext();

	// Setup sprite batch
	setSpriteBatch(new SpriteBatch(graphicsContext));

	//if(Config::isFullscreenEnabled())
	//{
	//	Window::enableFullscreen();
	//}

	getWindow()->setVSync(0);
	
	// Init world
	//BlockData::init();
	//BlockEntityData::init();

	EntityData::init(this);

	m_world = new World(this);
	addChildLast(m_world);

	// Initialize block and item data
	/*ItemData::init(this);

	// Setup debug
	m_debug = new Debug(this);

	// Setup commander
	m_commander = new Commander(this);

	// Resize the window
	getWindow()->setSize(1280, 720);

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
	
	// Create game state
	InGameState * state = new InGameState(this);
	//m_gameOverlay = new GameOverlay(this, state->getScene(), graphicsContext);

	// Push game state
	pushState(state);

	// Set key bindings
	/*InputContext *inputContext = Input::getContext("game");

	inputContext->bind("toggle_full_screen", bind(&OverworldGame::toggleFullscreen, this, placeholders::_1), true);
	inputContext->bind("take_screen_shot", bind(&OverworldGame::takeScreenshot, this, placeholders::_1), true);

	inputContext->bind("camera_zoom_in", bind(&Camera::zoomIn, m_world->getCamera(), placeholders::_1), true);
	inputContext->bind("camera_zoom_out", bind(&Camera::zoomOut, m_world->getCamera(), placeholders::_1), true);

	inputContext->bind("show_omnicon", bind(&Omnicon::toggle, m_gameOverlay->getOmnicon(), placeholders::_1), true);
	inputContext->bind("show_inventory", bind(&Inventory::toggle, m_gameOverlay->getInventory(), placeholders::_1), true);
	inputContext->bind("show_chat", bind(&Chat::toggle, m_gameOverlay->getChat(), placeholders::_1), true);

	inputContext->bind("hotbar_select_0", bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 0), true);
	inputContext->bind("hotbar_select_1", bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 1), true);
	inputContext->bind("hotbar_select_2", bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 2), true);
	inputContext->bind("hotbar_select_3", bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 3), true);
	inputContext->bind("hotbar_select_4", bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 4), true);
	inputContext->bind("hotbar_select_5", bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 5), true);
	inputContext->bind("hotbar_select_6", bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 6), true);
	inputContext->bind("hotbar_select_7", bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 7), true);
	inputContext->bind("hotbar_select_8", bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 8), true);
	inputContext->bind("hotbar_select_9", bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 9), true);

	inputContext->bind("next_block", bind(&Debug::nextBlock, m_debug, placeholders::_1), true);
	inputContext->bind("prev_block", bind(&Debug::prevBlock, m_debug, placeholders::_1), true);
	inputContext->bind("debug_func_1", bind(&Debug::debugFunction, m_debug, placeholders::_1, 1), true);
	inputContext->bind("debug_func_2", bind(&Debug::debugFunction, m_debug, placeholders::_1, 2), true);
	inputContext->bind("debug_func_3", bind(&Debug::debugFunction, m_debug, placeholders::_1, 3), true);
	inputContext->bind("debug_func_4", bind(&Debug::debugFunction, m_debug, placeholders::_1, 4), true);
	inputContext->bind("debug_func_5", bind(&Debug::debugFunction, m_debug, placeholders::_1, 5), true);
	inputContext->bind("debug_func_6", bind(&Debug::debugFunction, m_debug, placeholders::_1, 6), true);
	inputContext->bind("debug_func_7", bind(&Debug::debugFunction, m_debug, placeholders::_1, 7), true);
	inputContext->bind("debug_func_8", bind(&Debug::debugFunction, m_debug, placeholders::_1, 8), true);
	inputContext->bind("debug_func_9", bind(&Debug::debugFunction, m_debug, placeholders::_1, 9), true);
	inputContext->bind("debug_func_10", bind(&Debug::debugFunction, m_debug, placeholders::_1, 10), true);
	inputContext->bind("debug_func_11", bind(&Debug::debugFunction, m_debug, placeholders::_1, 11), true);
	inputContext->bind("debug_func_12", bind(&Debug::debugFunction, m_debug, placeholders::_1, 12), true);

	Input::setContext(inputContext);

	inputContext = Input::getContext("chat");
	inputContext->bind("send_message", bind(&Chat::sendMessage, m_gameOverlay->getChat(), placeholders::_1), true);
	inputContext->bind("escape_chat", bind(&Chat::toggle, m_gameOverlay->getChat(), placeholders::_1), true);*/
}

void OverworldGame::onEnd(GameEvent *e)
{
	// Save the world as we're exiting
	/*if(m_world) m_world->save();
	if(m_server) m_server->save();
	delete m_world;
	delete m_debug;
	delete m_commander;*/
}

void OverworldGame::pushState(GameState *state)
{/*
	assert(state);
	m_states.push_front(state);
	state->enter();*/
}

void OverworldGame::popState()
{/*
	if(!m_states.empty())
	{
		GameState *front = m_states.front();
		front->leave();
		m_states.remove(front);
		delete front;
	}
	else
	{
		end();
	}*/
}

GameState *OverworldGame::peekState(int level)
{/*
	if(m_states.size() == 0) return 0;
	list<GameState*>::iterator itr = m_states.begin();
	advance(itr, level);
	return *itr;
*/return 0;
}

void OverworldGame::onTick(TickEvent *e)
{
	// Update game connections
	/*if(Connection::getInstance()->isServer())
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
	m_debug->update(delta);*/
}

void OverworldGame::onDraw(DrawEvent *e)
{
	GraphicsContext *graphicsContext = e->getGraphicsContext();

	// Take screen shot
	if(m_takeScreenshot)
	{
		int i = 0;
		while(util::fileExists("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png")) i++;
		graphicsContext->saveScreenshot("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png");
		m_takeScreenshot = false;
	}

	// Draw game states
	/*for(list<GameState*>::iterator itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		(*itr)->draw(m_spriteBatch, alpha);
		if(!(*itr)->isTransparent())
		{
			break;
		}
	}*/

	// Render debug stuff
	//m_debug->setVariable("FPS", util::intToStr((int) getFPS()));
	//m_spriteBatch->begin();
	//m_debug->draw(m_spriteBatch, alpha);
	//m_spriteBatch->end();
}