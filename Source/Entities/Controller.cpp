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

void Controller::setClientInputState(const bool state, int type)
{
	m_clientInputState[type] = state;
}

void Controller::pack(RakNet::BitStream *bitStream, const Connection *conn)
{
	// If is local
	//if(m_local)
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