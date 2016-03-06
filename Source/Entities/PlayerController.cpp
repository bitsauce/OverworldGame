#include "PlayerController.h"
#include "Networking/Connection.h"

PlayerController::PlayerController(InputManager *input, const bool local)
{
	// If player is local, do extra stuff
	if(local)
	{
		InputContext *inputContext = input->getContextByName("game");
		//inputContext->getKeybind("activate_thing")->setFunction(bind(&Player::activateThing, this, placeholders::_1));
		inputContext->getKeybind("move_left")->setFunction(bind(&PlayerController::setClientInputState, this, placeholders::_1, INPUT_MOVE_LEFT));
		inputContext->getKeybind("move_right")->setFunction(bind(&PlayerController::setClientInputState, this, placeholders::_1, INPUT_MOVE_RIGHT));
		inputContext->getKeybind("jump")->setFunction(bind(&PlayerController::setClientInputState, this, placeholders::_1, INPUT_JUMP));
		inputContext->getKeybind("run")->setFunction(bind(&PlayerController::setClientInputState, this, placeholders::_1, INPUT_RUN));
		//inputContext->getKeybind("use_item")->setFunction(bind(&PlayerController::setClientUseItemState, this, placeholders::_1));
	}
}