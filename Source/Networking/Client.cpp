#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "PacketPriority.h"
#include "BitStream.h"
#include "NetworkIDObject.h"
#include "NetworkIDManager.h"

#include "Client.h"
#include "Constants.h"

Client::Client(const string &ip, const ushort port) :
	GameObject(DRAW_ORDER_CLIENT)
{
	m_rakPeer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor;
	assert(m_rakPeer->Startup(1, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
	assert(m_rakPeer->Connect(ip.c_str(), port, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);
}

void Client::update()
{

}