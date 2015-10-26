#include "PlayerController.h"
#include "Networking/Connection.h"

PlayerController::PlayerController(const bool local)
{
	// If player is local, do extra stuff
	if(local)
	{
		InputContext *inputContext = Input::getContext("game");
		//inputContext->bind("activate_thing", bind(&Player::activateThing, this, placeholders::_1), true);
		inputContext->bind("move_left", bind(&PlayerController::setClientInputState, this, placeholders::_1, INPUT_MOVE_LEFT), true);
		inputContext->bind("move_right", bind(&PlayerController::setClientInputState, this, placeholders::_1, INPUT_MOVE_RIGHT), true);
		inputContext->bind("jump", bind(&PlayerController::setClientInputState, this, placeholders::_1, INPUT_JUMP), true);
		inputContext->bind("run", bind(&PlayerController::setClientInputState, this, placeholders::_1, INPUT_RUN), true);
		inputContext->bind("use_item", bind(&PlayerController::setClientUseItemState, this, placeholders::_1), true);
	}
}