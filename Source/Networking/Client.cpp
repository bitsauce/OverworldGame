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

#include "Entities/Player.h"
#include "Entities/PlayerController.h"

#include "Gui/GameOverlay/GameOverlay.h"

Client::Client(OverworldGame *game, const string &ip, const ushort port) :
	Connection(false),
	m_game(game)
{
	RakNet::SocketDescriptor socketDescriptor;
	assert(m_rakPeer->Startup(1, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
	assert(m_rakPeer->Connect(ip.c_str(), port, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);

	m_rakPeer->SetTimeoutTime(600000, RakNet::UNASSIGNED_SYSTEM_ADDRESS); // For debugging purposes
}

void Client::onTick(TickEvent *e)
{
	for(NetworkObject *object : m_networkObjects)
	{
		if(object->m_local)
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
				BlockID blockID; bitStream.Read(blockID);
				WorldLayer layer; bitStream.Read(layer);
				m_game->getWorld()->getTerrain()->setBlockAt(x, y, layer, BlockData::get(blockID), true);
			}
			break;

			case ID_PLAYER_JOIN:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				char *playerName = new char[MAX_USERNAME_LENGTH]; bitStream.Read(playerName);
				RakNet::RakNetGUID guid; bitStream.Read(guid);
				RakNet::NetworkID playerNetworkID; bitStream.Read(playerNetworkID);
				RakNet::NetworkID controllerNetworkID; bitStream.Read(controllerNetworkID);

				// Create player
				Json::Value attributes;
				attributes["name"] = playerName;
				Player *player = new Player(attributes);
				player->SetNetworkIDManager(&m_networkIDManager);
				player->SetNetworkID(playerNetworkID);
				player->unpack(&bitStream, this);

				// Create player controller
				PlayerController *playerController = new PlayerController(m_game, true);
				playerController->SetNetworkIDManager(&m_networkIDManager);
				playerController->SetNetworkID(controllerNetworkID);
				player->setController(playerController);

				player->m_local = true;
				playerController->m_local = true;

				m_game->getGameOverlay()->setPlayer(player);
				m_game->getWorld()->getCamera()->setTargetEntity(player);
				m_game->getWorld()->m_localPlayer = player;
			}
			break;

			case ID_CREATE_ENTITY:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				EntityID entityID; bitStream.Read(entityID);
				RakNet::NetworkID networkID; bitStream.Read(networkID);
				RakNet::RakNetGUID guid; bitStream.Read(guid);

				switch(entityID)
				{
					case 1:
					{
						// Create player
						Player *player = new Player(Json::Value());
						player->SetNetworkIDManager(&m_networkIDManager);
						player->SetNetworkID(networkID);
						player->setController(new PlayerController(m_game, false));
					}
					break;
				}
			}
			break;

			case ID_NETWORK_OBJECT_UPDATE:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				RakNet::NetworkID id; bitStream.Read(id);
				NetworkObject *object = m_networkIDManager.GET_OBJECT_FROM_ID<NetworkObject*>(id);
				if(object)
				{
					object->unpack(&bitStream, this);
				}
			}
			break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				m_game->getWorld()->clear();

				// Connection accepted, join as player
				RakNet::BitStream bitStream;
				bitStream.Write((RakNet::MessageID) ID_PLAYER_JOIN);
				bitStream.Write("BitsauceClient");
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