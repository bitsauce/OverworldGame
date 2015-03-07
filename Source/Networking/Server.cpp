#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "PacketPriority.h"
#include "BitStream.h"
#include "NetworkIDObject.h"
#include "NetworkIDManager.h"
#include "PacketLogger.h"

#include "Server.h"
#include "NetworkObject.h"
#include "Constants.h"
#include "World/World.h"
#include "Terrain/Terrain.h"
#include "Entities/Player.h"
#include "Physics/PhysicsBody.h"

Server::Server(World &world, const ushort port) :
	Connection(true),
	m_world(world)
{
	m_rakPeer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor socketDescriptor(port, 0);
	if(m_rakPeer->Startup(2, &socketDescriptor, 1) != RakNet::RAKNET_STARTED)
	{
		socketDescriptor.port = 0;
		assert(m_rakPeer->Startup(2, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
	}
	m_rakPeer->SetMaximumIncomingConnections(2);
}

void Server::update()
{
	m_isServer = false;
	for(NetworkObject *object : m_networkObjects)
	{
		//if(object->isLocal())
		{
			RakNet::BitStream bitStream;
			bitStream.Write((RakNet::MessageID)ID_NETWORK_OBJECT_UPDATE);
			bitStream.Write(object->GetNetworkID());
			object->pack(&bitStream, this);
			m_rakPeer->SendLoopback((const char*)bitStream.GetData(), bitStream.GetNumberOfBytesUsed());
		}
	}
	m_isServer = true;

	for(RakNet::Packet *packet = m_rakPeer->Receive(); packet; m_rakPeer->DeallocatePacket(packet), packet = m_rakPeer->Receive())
	{
		switch(packet->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			LOG("Client connected from %s with GUID %s", packet->systemAddress.ToString(true), packet->guid.ToString());

			for(NetworkObject *object : m_networkObjects)
			{
				RakNet::BitStream bitStream;
				bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
				bitStream.Write(object->GetNetworkID());
				bitStream.Write(m_rakPeer->GetMyGUID()/*object->GetGUID()*/);
				sendPacket(&bitStream);
			}

			break;

		case ID_SET_BLOCK:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				int x; bitStream.Read(x);
				int y; bitStream.Read(y);
				BlockID block; bitStream.Read(block);
				TerrainLayer layer; bitStream.Read(layer);
				m_world.getTerrain()->setBlockAt(x, y, block, layer);
			}
			break;

		case ID_CREATE_ENTITY:
			{
				LOG("Creating player...");

				// Create player
				Player *player = new Player(m_world, packet->guid);
				player->SetNetworkIDManager(&m_networkIDManager);
				m_networkObjects.push_back(player);

				player->getBody().setPosition(0, 0);
				player->getItemContainer().addItem(ITEM_PICKAXE_IRON);
				player->getItemContainer().addItem(ITEM_TORCH, 255);
				player->getItemContainer().addItem(ITEM_BOW_WOODEN);
				
				// Brodcast the packet to all clients with the network id of the object added
				RakNet::BitStream bitStream(packet->data, packet->length, true);
				bitStream.Write(player->GetNetworkID());
				bitStream.Write(packet->guid);
				sendPacket(&bitStream);
			}
			break;
			
		case ID_NETWORK_OBJECT_UPDATE:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				RakNet::NetworkID networkID; bitStream.Read(networkID);
				NetworkObject *object = m_networkIDManager.GET_OBJECT_FROM_ID<NetworkObject*>(networkID);
				if(object)
				{
					object->unpack(&bitStream, this);
					//object->update();

					RakNet::BitStream outStream;
					outStream.Write((RakNet::MessageID)ID_NETWORK_OBJECT_UPDATE);
					outStream.Write(object->GetNetworkID());
					object->pack(&outStream, this);
					sendPacket(&outStream);
				}
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