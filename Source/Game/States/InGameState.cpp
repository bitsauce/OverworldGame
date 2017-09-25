#include "InGameState.h"
#include "Game/Game.h"
#include "Gui/GameOverlay/GameOverlay.h"

InGameState::InGameState(Overworld *game, World *world) :
	GameState(GAME_STATE_IN_GAME, false),
	m_gameOverlay(nullptr),
	m_world(world)
{
	// Create game overlay
	m_gameOverlay = new GameOverlay(game, game->getWindow());
	m_gameOverlay->setPlayer(world->getLocalPlayer());

	addChildLast(m_world);
	if(game->getServer()->getWorld()) addChildLast(game->getServer());
	if(game->getClient()->getWorld()) addChildLast(game->getClient());
	addChildLast(m_gameOverlay);

	// Create debugger object
	m_debug = new InGameDebug(this);
	addChildLast(m_debug);

	InputContext *inputContext = game->getInputManager()->getContextByName("game");

	inputContext->getKeybind("show_chat")->setFunction(bind(&Chat::toggle, m_gameOverlay->getChat(), placeholders::_1));

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

	inputContext->getKeybind("camera_zoom_in")->setFunction(bind(&Camera::zoomIn, m_world->getCamera(), placeholders::_1));
	inputContext->getKeybind("camera_zoom_out")->setFunction(bind(&Camera::zoomOut, m_world->getCamera(), placeholders::_1));

	inputContext->getKeybind("debug_func_1")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_2")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_3")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_4")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_5")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_6")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_7")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_8")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_9")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_10")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_11")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("debug_func_12")->setFunction(bind(&InGameDebug::debugFunction, m_debug, placeholders::_1));
	inputContext->getKeybind("next_block")->setFunction(bind(&InGameDebug::nextBlock, m_debug, placeholders::_1));
	inputContext->getKeybind("prev_block")->setFunction(bind(&InGameDebug::prevBlock, m_debug, placeholders::_1));
	//inputContext->getKeybind("place_light")->setFunction(bind(&InGameDebug::placeLight, m_debug, placeholders::_1));
	//inputContext->getKeybind("randomize_light")->setFunction(bind(&InGameDebug::randomizeLight, m_debug, placeholders::_1));

	inputContext = game->getInputManager()->getContextByName("chat");
	inputContext->getKeybind("send_message")->setFunction(bind(&Chat::sendMessage, m_gameOverlay->getChat(), placeholders::_1));
	inputContext->getKeybind("next_message")->setFunction(bind(&Chat::nextMessage, m_gameOverlay->getChat(), placeholders::_1));
	inputContext->getKeybind("prev_message")->setFunction(bind(&Chat::prevMessage, m_gameOverlay->getChat(), placeholders::_1));
	inputContext->getKeybind("escape_chat")->setFunction(bind(&Chat::toggle, m_gameOverlay->getChat(), placeholders::_1));
}

void InGameState::onTransitionBegin()
{
}
