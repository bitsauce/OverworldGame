#include "PlayerController.h"
#include "Networking/Connection.h"
#include "Gui/GameOverlay/GameOverlay.h"
#include "Game/Game.h"

PlayerController::PlayerController(OverworldGame *game, const bool local) :
	m_game(game)
{
	// If player is local, do extra stuff
	if(local)
	{
		InputManager *input = m_game->getInputManager();
		InputContext *inputContext = input->getContextByName("game");
		//inputContext->getKeybind("activate_thing")->setFunction(bind(&Player::activateThing, this, placeholders::_1));
		inputContext->getKeybind("move_left")->setFunction(bind(&PlayerController::updateInputState, this, placeholders::_1, INPUT_MOVE_LEFT));
		inputContext->getKeybind("move_right")->setFunction(bind(&PlayerController::updateInputState, this, placeholders::_1, INPUT_MOVE_RIGHT));
		inputContext->getKeybind("jump")->setFunction(bind(&PlayerController::updateInputState, this, placeholders::_1, INPUT_JUMP));
		inputContext->getKeybind("run")->setFunction(bind(&PlayerController::updateInputState, this, placeholders::_1, INPUT_RUN));
		inputContext->getKeybind("use_item")->setFunction(bind(&PlayerController::updateUseItemState, this, placeholders::_1));
	}
}

PlayerController::~PlayerController()
{
	if(m_local)
	{
		InputManager *input = m_game->getInputManager();
		InputContext *inputContext = input->getContextByName("game");
		//inputContext->getKeybind("activate_thing")->setFunction(function<void(KeyEvent*)>());
		inputContext->getKeybind("move_left")->setFunction(function<void(KeyEvent*)>());
		inputContext->getKeybind("move_right")->setFunction(function<void(KeyEvent*)>());
		inputContext->getKeybind("jump")->setFunction(function<void(KeyEvent*)>());
		inputContext->getKeybind("run")->setFunction(function<void(KeyEvent*)>());
		inputContext->getKeybind("use_item")->setFunction(function<void(KeyEvent*)>());
	}
}

void PlayerController::updateUseItemState(KeyEvent *e)
{
	m_inputState[INPUT_USE_ITEM] = e->getType() != KeyEvent::UP && !m_game->getGameOverlay()->isHovered();
}

void PlayerController::updateInputState(KeyEvent *e, int type)
{
	m_inputState[type] = e->getType() != KeyEvent::UP;
}