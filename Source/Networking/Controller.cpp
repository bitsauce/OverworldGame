#include "Controller.h"
#include "Networking/Connection.h"
#include "Game/Game.h"

Controller::Controller() :
	m_inputState(0)
{
}

void Controller::setInputState(const bool state, int type)
{
	m_inputState ^= (-(state ? 1 : 0) ^ m_inputState) & (1 << type);
}