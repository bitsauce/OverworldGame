#include "Controller.h"
#include "Networking/Connection.h"

Controller::Controller()
{
	// Set input states to false
	for(uint i = 0; i < INPUT_COUNT; ++i)
	{
		m_inputState[i] = false;
	}
}

void Controller::setInputState(const bool state, int type)
{
	m_inputState[type] = state;
}

void Controller::pack(RakNet::BitStream *bitStream, const Connection *conn)
{
	for(uint i = 0; i < INPUT_COUNT; ++i)
	{
		bitStream->Write(m_inputState[i]);
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