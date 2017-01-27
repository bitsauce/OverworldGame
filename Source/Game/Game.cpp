#include "Game.h"

#include "Constants.h"
#include "Gui/Canvas.h"
#include "Gui/UiObject.h"
#include "Blocks/BlockData.h"
#include "Items/ItemData.h"
#include "Game/Debug.h"
#include "World/World.h"
#include "Networking/Server.h"
#include "Networking/Client.h"
#include "Entities/Entity.h"
#include "Entities/EntityData.h"
#include "BlockEntities/BlockEntityData.h"
#include "States/InGameState.h"
#include "Gui/GameOverlay/GameOverlay.h"

OverworldGame::OverworldGame() :
	Game("Overworld", SAUCE_EXPORT_LOG | SAUCE_RUN_IN_BACKGROUND | SAUCE_WINDOW_RESIZABLE),
	m_world(nullptr),
	m_takeScreenshot(false),
	m_gameOverlay(nullptr),
	m_debug(nullptr),
	m_commander(nullptr)
{
}

void OverworldGame::onStart(GameEvent *e)
{
	GraphicsContext *graphicsContext = getWindow()->getGraphicsContext();

	// Setup sprite batch
	m_spriteBatch = new SpriteBatch(graphicsContext);

	//if(Config::isFullscreenEnabled())
	//{
	//	Window::enableFullscreen();
	//}

	getWindow()->setVSync(0);
	
	// Inititialze data
	BlockData::init();
	BlockEntityData::init();
	EntityData::init(this);
	ItemData::init(this);

	m_world = new World(this);
	//addChildLast(m_world);

	// Setup debug
	m_debug = new Debug(this);
	addChildLast(m_debug);

	// Setup commander
	m_commander = new Commander(this);

	// Create GUI canvas
	m_canvas = new Canvas(getWindow(), 1280, 720);
	addChildLast(m_canvas);

	// Create game overlay
	m_gameOverlay = new GameOverlay(this, m_canvas, graphicsContext);

	// Load world "Debug", or create it if it doesn't exists
	if(!m_world->load("DebugLighting"))
	{
		m_world->create("DebugLighting");
	}
	
	LOG("Hosting local server on port '45556'...");

	// Create server object
	Server *server = new Server(this, 45556);
	
	// Join server as client
	RakNet::BitStream bitStream;
	bitStream.Write((RakNet::MessageID) ID_PLAYER_JOIN);
	bitStream.Write("Bitsauce");
	server->getRakPeer()->SendLoopback((const char*) bitStream.GetData(), bitStream.GetNumberOfBytesUsed());

	// Create game state
	InGameState *state = new InGameState(this, server);

	// Push game state
	pushState(state);

	// Setup keys
	initKeybindings();
}

void OverworldGame::onEnd(GameEvent *e)
{
	// Save the world as we're exiting
	if(m_world) m_world->save();
	//if(m_server) m_server->save();
	delete m_world;
	delete m_debug;
	delete m_commander;
}

void OverworldGame::pushState(GameState *state)
{
	if(state)
	{
		addChildFirst(state);
		state->onEnter();
	}
}

void OverworldGame::popState()
{
	((GameState*) getChildren().front())->onLeave();
	removeChildFront();
	if(getChildren().empty()) end();
}

GameState *OverworldGame::peekState(int level)
{
	if(getChildren().empty()) return 0;
	list<SceneObject*>::iterator itr = getChildren().begin();
	advance(itr, level);
	return (GameState*) *itr;
}

void OverworldGame::initKeybindings()
{
	// Set key bindings
	/*InputContext *inputContext = Input::getContext("game");

	inputContext->bind("toggle_full_screen"->setFunction(bind(&OverworldGame::toggleFullscreen, this, placeholders::_1), true);

	inputContext->bind("show_omnicon"->setFunction(bind(&Omnicon::toggle, m_gameOverlay->getOmnicon(), placeholders::_1), true);
	inputContext->bind("show_inventory"->setFunction(bind(&Inventory::toggle, m_gameOverlay->getInventory(), placeholders::_1), true);

	Input::setContext(inputContext);*/

	InputContext *inputContext = getInputManager()->getContextByName("game");

	inputContext->getKeybind("camera_zoom_in")->setFunction(bind(&Camera::zoomIn, m_world->getCamera(), placeholders::_1));
	inputContext->getKeybind("camera_zoom_out")->setFunction(bind(&Camera::zoomOut, m_world->getCamera(), placeholders::_1));

	inputContext->getKeybind("show_chat")->setFunction(bind(&Chat::toggle, m_gameOverlay->getChat(), placeholders::_1));

	inputContext->getKeybind("take_screen_shot")->setFunction(bind(&OverworldGame::takeScreenshot, this, placeholders::_1));

	inputContext->getKeybind("hotbar_select_0")->setFunction(bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 0));
	inputContext->getKeybind("hotbar_select_1")->setFunction(bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 1));
	inputContext->getKeybind("hotbar_select_2")->setFunction(bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 2));
	inputContext->getKeybind("hotbar_select_3")->setFunction(bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 3));
	inputContext->getKeybind("hotbar_select_4")->setFunction(bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 4));
	inputContext->getKeybind("hotbar_select_5")->setFunction(bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 5));
	inputContext->getKeybind("hotbar_select_6")->setFunction(bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 6));
	inputContext->getKeybind("hotbar_select_7")->setFunction(bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 7));
	inputContext->getKeybind("hotbar_select_8")->setFunction(bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 8));
	inputContext->getKeybind("hotbar_select_9")->setFunction(bind(&Hotbar::setSelectedSlot, m_gameOverlay->getHotbar(), placeholders::_1, 9));

	inputContext->getKeybind("debug_func_1")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_2")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_3")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_4")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_5")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_6")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_7")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_8")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_9")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_10")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_11")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_12")->setFunction(bind(&Debug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("next_block")->setFunction(bind(&Debug::nextBlock, m_debug, placeholders::_1));
	inputContext->getKeybind("prev_block")->setFunction(bind(&Debug::prevBlock, m_debug, placeholders::_1));
	//inputContext->getKeybind("place_light")->setFunction(bind(&Debug::placeLight, m_debug, placeholders::_1));
	//inputContext->getKeybind("randomize_light")->setFunction(bind(&Debug::randomizeLight, m_debug, placeholders::_1));

	getInputManager()->setContext(inputContext);

	inputContext = getInputManager()->getContextByName("chat");
	inputContext->getKeybind("send_message")->setFunction(bind(&Chat::sendMessage, m_gameOverlay->getChat(), placeholders::_1));
	inputContext->getKeybind("next_message")->setFunction(bind(&Chat::nextMessage, m_gameOverlay->getChat(), placeholders::_1));
	inputContext->getKeybind("prev_message")->setFunction(bind(&Chat::prevMessage, m_gameOverlay->getChat(), placeholders::_1));
	inputContext->getKeybind("escape_chat")->setFunction(bind(&Chat::toggle, m_gameOverlay->getChat(), placeholders::_1));
}

void OverworldGame::onTick(TickEvent *e)
{
	SceneObject::onTick(e);
}

void OverworldGame::onDraw(DrawEvent *e)
{
	GraphicsContext *graphicsContext = e->getGraphicsContext();
	e->setUserData(m_spriteBatch);

	// Take screen shot
	if(m_takeScreenshot)
	{
		int i = 0;
		while(util::fileExists("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png")) i++;
		graphicsContext->saveScreenshot("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png");
		m_takeScreenshot = false;
	}

	m_spriteBatch->begin();

	SceneObject::onDraw(e);

	m_spriteBatch->end();

	// Draw game states
	/*for(list<GameState*>::iterator itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		(*itr)->draw(m_spriteBatch, alpha);
		if(!(*itr)->isTransparent())
		{
			break;
		}
	}*/
}