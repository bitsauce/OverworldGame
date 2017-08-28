#include "Game.h"

#include "Constants.h"
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
#include "States/TestMenu.h"
#include "Gui/Gui.h"
#include "Gui/GameOverlay/GameOverlay.h"

Overworld *Overworld::s_this = 0;

// TODO:
// [x] Fix it so that the game doesn't load chunks around 0, 0 at the start
// [ ] Loading screen
// [ ] Work on separating the client and server
//     [ ] ChunkManager
// [ ] Fix it so that the "in-game" game state can be drawn to a render target
// [ ] Bring the GameOverlay elements back

Overworld::Overworld() :
	Game("Overworld", SAUCE_EXPORT_LOG | SAUCE_RUN_IN_BACKGROUND | SAUCE_WINDOW_RESIZABLE),
	m_takeScreenshot(false),
	m_gameOverlay(nullptr),
	m_debug(nullptr),
	m_commander(nullptr),
	m_fadeTime(0.5f),
	m_transitionTime(0.0f),
	m_transitionDirection(0)
{
	s_this = this;
}

void Overworld::onStart(GameEvent *e)
{
	GraphicsContext *graphicsContext = getWindow()->getGraphicsContext();

	// Setup sprite batch
	m_spriteBatch = new SpriteBatch();

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

	// Create commander
	m_commander = new Commander(this);

	// Create client and server object
	m_client = new Client(this);
	m_server = new Server(this);

	// Create debug object
	m_debug = new Debug(this);
	addChildLast(m_debug);

	// Create game overlay
	Canvas *canvas = new Canvas(getWindow());
	m_gameOverlay = new GameOverlay(this, canvas, graphicsContext);

	// Go to test menu
	pushState(new TestMenu(this));

	// Setup keys
	initKeybindings();
}

void Overworld::onEnd(GameEvent *e)
{
	// Save the world as we're exiting
	if(m_server) m_server->save();
	delete m_debug;
	delete m_commander;
}

void Overworld::pushState(GameState *state)
{
	m_states.push_front(state);
	m_transitionTime = m_fadeTime;
	m_transitionDirection = 1;

	// Send a window resize event to make sure all the elements of the canvas are resized correctly
	WindowEvent e(WindowEvent::SIZE_CHANGED, Game::Get()->getWindow(), Game::Get()->getWindow()->getWidth(), Game::Get()->getWindow()->getHeight());
	state->onEvent(&e);

	state->onTransitionBegin();
}

bool Overworld::popState()
{
	// If there are more than 2 game states
	if(m_states.size() >= 2)
	{
		// Setup transition
		m_transitionTime = m_fadeTime;
		m_transitionDirection = -1;

		// Send a window resize event to make sure all the elements of the canvas are resized correctly
		WindowEvent e(WindowEvent::SIZE_CHANGED, Game::Get()->getWindow(), Game::Get()->getWindow()->getWidth(), Game::Get()->getWindow()->getHeight());
		(*++m_states.begin())->onEvent(&e);
		return false;
	}
	m_states.pop_front();
	return true;
}

GameState *Overworld::peekState(int level)
{
	if(getChildren().empty()) return 0;
	list<SceneObject*>::iterator itr = getChildren().begin();
	advance(itr, level);
	return (GameState*) *itr;
}

void Overworld::initKeybindings()
{
	// Set key bindings
	/*InputContext *inputContext = Input::getContext("game");

	inputContext->bind("toggle_full_screen"->setFunction(bind(&Overworld::toggleFullscreen, this, placeholders::_1), true);

	inputContext->bind("show_omnicon"->setFunction(bind(&Omnicon::toggle, m_gameOverlay->getOmnicon(), placeholders::_1), true);
	inputContext->bind("show_inventory"->setFunction(bind(&Inventory::toggle, m_gameOverlay->getInventory(), placeholders::_1), true);

	Input::setContext(inputContext);*/

	InputContext *inputContext = getInputManager()->getContextByName("game");

	//inputContext->getKeybind("camera_zoom_in")->setFunction(bind(&Camera::zoomIn, m_world->getCamera(), placeholders::_1));
	//inputContext->getKeybind("camera_zoom_out")->setFunction(bind(&Camera::zoomOut, m_world->getCamera(), placeholders::_1));

	inputContext->getKeybind("show_chat")->setFunction(bind(&Chat::toggle, m_gameOverlay->getChat(), placeholders::_1));

	inputContext->getKeybind("take_screen_shot")->setFunction(bind(&Overworld::takeScreenshot, this, placeholders::_1));

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

void Overworld::onTick(TickEvent *e)
{
	m_transitionTime -= e->getDelta();
	if(m_transitionDirection != 0 && m_transitionTime <= 0.0f)
	{
		if(m_transitionDirection == -1)
		{
			m_states.pop_front();

			// Send a mouse move event to make sure the previous state is reset
			Vector2F position = Game::Get()->getInputManager()->getPosition();
			MouseEvent e(MouseEvent::MOVE, Game::Get()->getInputManager(), position.x, position.y, SAUCE_MOUSE_BUTTON_NONE, 0, 0);
			m_states.front()->onEvent(&e);
		}
		m_transitionDirection = 0;
	}
	SceneObject::onTick(e);
}

void Overworld::onDraw(DrawEvent *e)
{
	// Setup render event
	GraphicsContext *graphicsContext = e->getGraphicsContext();
	e->setUserData(m_spriteBatch);
	m_spriteBatch->begin(graphicsContext);

	// Take a screenshot
	if(m_takeScreenshot)
	{
		int i = 0;
		while(util::fileExists("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png")) i++;
		graphicsContext->saveScreenshot("C:\\Users\\Marcus\\Desktop\\Screenshot_" + util::intToStr(i) + ".png");
		m_takeScreenshot = false;
	}

	// Draw the game states from top to bottom until a fully opaque layer is drawn
	list<GameState*>::const_iterator itr = m_states.begin();
	while(itr != m_states.end() && ((*itr)->isTransparent() || m_transitionTime > 0.0f)) itr++;
	if(itr == m_states.end()) itr--;
	while(true)
	{
		GameState *gameState = *itr;
		gameState->onEvent(e);

		float alpha = itr == m_states.begin() ? (m_transitionDirection == -1 ? max(m_transitionTime / m_fadeTime, 0.0f) : min(1.f - (m_transitionTime / m_fadeTime), 1.0f)) : 1.0f;

		// Draw canvas
		Vector2I canvasSize = getWindow()->getSize();//gameState->getSize();
		graphicsContext->setTexture(gameState->getRenderTarget()->getTexture());
		graphicsContext->drawRectangle(0, 0, canvasSize.x, canvasSize.y, Color(255, 255, 255, 255 * alpha));
		graphicsContext->setTexture(0);

		if(itr == m_states.begin())
		{
			break;
		}
		itr--;
	}

	m_spriteBatch->end();
}

void Overworld::onEvent(Event *e)
{
	// Pass the event if it's not a draw event
	if(e->getType() != EVENT_DRAW)
	{
		if(e->getType() == EVENT_WINDOW_SIZE_CHANGED)
		{
			// This accounts for the fringe case where we popped a non-transparent canvas and we resize
			// the window during the trasition. In this case we should ignore the first non-transparent canvas
			int transparentCounter = (m_transitionDirection != 0 && !m_states.front()->isTransparent()) ? 2 : 1;
			for(GameState *state : m_states)
			{
				state->onEvent(e);
				if(!state->isTransparent())
				{
					if(--transparentCounter == 0)
					{
						break;
					}
				}
			}
		}
		else
		{
			if(!m_states.empty())
			{
				m_states.front()->onEvent(e);
			}
		}
	}
	SceneObject::onEvent(e);
}