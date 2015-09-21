#include "Controller.h"
#include "Networking/Connection.h"

Controller::Controller()
{
	// Set input states to false
	for(uint i = 0; i < INPUT_COUNT; ++i)
	{
		m_clientInputState[i] = m_inputState[i] = false;
	}
}

void Controller::setClientUseItemState(int action)
{
	m_clientInputState[INPUT_USE_ITEM] = action == GLFW_PRESS;// && !m_gameOverlay->isHovered();
}

void Controller::setClientInputState(int action, int type)
{
	m_clientInputState[type] = action == GLFW_PRESS;
}

void Controller::pack(RakNet::BitStream *bitStream, const Connection *conn)
{
	// If is local
	//if(conn->getGUID() == m_guid)
	{
		for(uint i = 0; i < INPUT_COUNT; ++i)
		{
			bitStream->Write(m_clientInputState[i]);
		}
	}
}

void Controller::unpack(RakNet::BitStream *bitStream, const Connection *conn)
{
	if(conn->isServer())
	{
		for(uint i = 0; i < INPUT_COUNT; ++i)
		{
			bitStream->Read(m_inputState[i]);
		}
	}
}