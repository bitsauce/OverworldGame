#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "PacketPriority.h"
#include "BitStream.h"
#include "NetworkIDObject.h"
#include "NetworkIDManager.h"
#include "PacketLogger.h"

#include "Server.h"
#include "Constants.h"

Server::Server(const ushort port) :
	GameObject(DRAW_ORDER_SERVER)
{
	m_rakPeer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor(port, 0);
	assert(m_rakPeer->Startup(2, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
	m_rakPeer->SetMaximumIncomingConnections(2);
}

void Server::update()
{
	for(RakNet::Packet *packet = m_rakPeer->Receive(); packet; m_rakPeer->DeallocatePacket(packet), packet = m_rakPeer->Receive())
	{
		switch(packet->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			LOG("Client connected from %s with GUID %s", packet->systemAddress.ToString(true), packet->guid.ToString());
			break;

		default:
			LOG("Received packet type %s", RakNet::PacketLogger::BaseIDTOString(packet->data[0]));
			break;
		}
	}
}