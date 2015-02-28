#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "PacketPriority.h"
#include "BitStream.h"
#include "NetworkIDObject.h"
#include "NetworkIDManager.h"
#include "PacketLogger.h"

#include "Client.h"
#include "Constants.h"

#include "World/World.h"
#include "Terrain/Terrain.h"

Client *Client::s_instance = nullptr;

Client::Client(const string &ip, const ushort port) :
	GameObject(DRAW_ORDER_CLIENT)
{
	s_instance = this;
	m_rakPeer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor;
	assert(m_rakPeer->Startup(1, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
	assert(m_rakPeer->Connect(ip.c_str(), port, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);
}

void Client::update()
{
	for(NetworkIDObject)
	{
		object->pack();
	}

	for(RakNet::Packet *packet = m_rakPeer->Receive(); packet; m_rakPeer->DeallocatePacket(packet), packet = m_rakPeer->Receive())
	{
		switch(packet->data[0])
		{
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

		case ID_PLAYER_CREATE:
			{

			}
			break;

		default:
			LOG("Received packet type %s", RakNet::PacketLogger::BaseIDTOString(packet->data[0]));
			break;
		}
	}
}

void Client::sendPacket(RakNet::BitStream *bitStream)
{
	assert(m_rakPeer->Send(bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
}