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
#include "World/World.h"
#include "Terrain/Terrain.h"

Server *Server::s_instance = nullptr;

Server::Server(const ushort port) :
	GameObject(DRAW_ORDER_SERVER)
{
	setInstance(this);
	m_rakPeer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor(port, 0);
	assert(m_rakPeer->Startup(2, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
	m_rakPeer->SetMaximumIncomingConnections(2);
}

void Server::update()
{
	for(NetworkIDObject)
	{
		object->unpack();
	}

	for(RakNet::Packet *packet = m_rakPeer->Receive(); packet; m_rakPeer->DeallocatePacket(packet), packet = m_rakPeer->Receive())
	{
		switch(packet->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			LOG("Client connected from %s with GUID %s", packet->systemAddress.ToString(true), packet->guid.ToString());
			break;

		case ID_SET_BLOCK:
			{
			RakNet::BitStream bitStream(packet->data, packet->length, false);
			bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
			int x; bitStream.Read(x);
			int y; bitStream.Read(y);
			BlockID block; bitStream.Read(block);
			TerrainLayer layer; bitStream.Read(layer);
			World::getTerrain()->setBlockAt(x, y, block, layer);
			}
			break;

		default:
			LOG("Received packet type %s", RakNet::PacketLogger::BaseIDTOString(packet->data[0]));
			break;
		}
	}
}

void Server::sendPacket(RakNet::BitStream *bitStream)
{
	assert(m_rakPeer->Send(bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
}