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
#include "NetworkObject.h"

#include "Game/Game.h"
#include "World/World.h"

#include "Entities/Pawn.h"

Client::Client(OverworldGame * game, const string &ip, const ushort port) :
	Connection(false),
	m_game(game)
{
	RakNet::SocketDescriptor socketDescriptor;
	assert(m_rakPeer->Startup(1, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
	assert(m_rakPeer->Connect(ip.c_str(), port, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);
}

void Client::update()
{
	for(NetworkObject *object : m_networkObjects)
	{
		RakNet::BitStream bitStream;
		bitStream.Write((RakNet::MessageID)ID_NETWORK_OBJECT_UPDATE);
		bitStream.Write(object->GetNetworkID());
		uint bitsBefore = bitStream.GetNumberOfBitsUsed();
		object->pack(&bitStream, this);
		if(bitsBefore < bitStream.GetNumberOfBitsUsed())
		{
			sendPacket(&bitStream);
		}
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
				WorldLayer layer; bitStream.Read(layer);
				m_game->getWorld()->getTerrain()->setBlockAt(x, y, block, layer);
			}
			break;

		case ID_CREATE_ENTITY:
			{
				/*RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				//bitStream.Read(entityID);
				RakNet::NetworkID networkID; bitStream.Read(networkID);
				RakNet::RakNetGUID guid; bitStream.Read(guid);

				// Create player
				Pawn *player = new Pawn(m_game, guid);
				player->SetNetworkIDManager(&m_networkIDManager);
				player->SetNetworkID(networkID);

				m_networkObjects.push_back(player);

				player->setPosition(Vector2F(0, 0));
				player->getStorage()->addItem(ITEM_PICKAXE_IRON);
				player->getStorage()->addItem(ITEM_TORCH, 255);*/
			}
			break;
			
		case ID_NETWORK_OBJECT_UPDATE:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				RakNet::NetworkID id; bitStream.Read(id);
				NetworkObject *object = m_networkIDManager.GET_OBJECT_FROM_ID<NetworkObject*>(id);
				if(object) {
					object->unpack(&bitStream, this);
				}
			}
			break;

		case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				RakNet::BitStream bitStream;
				bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
				//bitStream.Write(ENTITY_PLAYER);
				sendPacket(&bitStream);
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