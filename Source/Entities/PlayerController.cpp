#include "PlayerController.h"
#include "Networking/Connection.h"

PlayerController::PlayerController(InputManager *input, const bool local)
{
	// If player is local, do extra stuff
	if(local)
	{
		InputContext *inputContext = input->getContextByName("game");
		//inputContext->getKeybind("activate_thing")->setFunction(bind(&Player::activateThing, this, placeholders::_1));
		inputContext->getKeybind("move_left")->setFunction(bind(&PlayerController::updateClientInputState, this, placeholders::_1, INPUT_MOVE_LEFT));
		inputContext->getKeybind("move_right")->setFunction(bind(&PlayerController::updateClientInputState, this, placeholders::_1, INPUT_MOVE_RIGHT));
		inputContext->getKeybind("jump")->setFunction(bind(&PlayerController::updateClientInputState, this, placeholders::_1, INPUT_JUMP));
		inputContext->getKeybind("run")->setFunction(bind(&PlayerController::updateClientInputState, this, placeholders::_1, INPUT_RUN));
		inputContext->getKeybind("use_item")->setFunction(bind(&PlayerController::updateClientUseItemState, this, placeholders::_1));
	}
}

void PlayerController::updateClientUseItemState(KeyEvent *e)
{
	m_clientInputState[INPUT_USE_ITEM] = e->getType() != KeyEvent::UP;// && !m_gameOverlay->isHovered();
}

void PlayerController::updateClientInputState(KeyEvent *e, int type)
{
	m_clientInputState[type] = e->getType() != KeyEvent::UP;
}