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
#include "Entities/Pawn.h"
#include "Entities/Player.h"
#include "Entities/PlayerController.h"
#include "Entities/AIController.h"
#include "Entities/Mobs/Zombie.h"
#include "Game/Game.h"
#include "Gui/GameOverlay/GameOverlay.h"

Server::Server(OverworldGame * game, const ushort port) :
	Connection(true),
	m_game(game)
{
	RakNet::SocketDescriptor socketDescriptor(port, 0);
	if(m_rakPeer->Startup(2, &socketDescriptor, 1) != RakNet::RAKNET_STARTED)
	{
		socketDescriptor.port = 0;
		assert(m_rakPeer->Startup(2, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
	}
	m_rakPeer->SetMaximumIncomingConnections(2);
}

void Server::onTick(TickEvent *e)
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
			m_rakPeer->SendLoopback((const char*) bitStream.GetData(), bitStream.GetNumberOfBytesUsed());
		}
	}
	m_isServer = true;

	for(RakNet::Packet *packet = m_rakPeer->Receive(); packet; m_rakPeer->DeallocatePacket(packet), packet = m_rakPeer->Receive())
	{
		switch(packet->data[0])
		{
			case ID_NEW_INCOMING_CONNECTION:
			{
				LOG("Client connected from %s with GUID %s", packet->systemAddress.ToString(true), packet->guid.ToString());

				for(NetworkObject *object : m_networkObjects)
				{
					RakNet::BitStream bitStream;
					bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
					bitStream.Write(object->GetNetworkID());
					bitStream.Write(m_rakPeer->GetMyGUID()/*object->GetGUID()*/);
					sendPacket(&bitStream);
				}
			}
			break;

			case ID_PLAYER_JOIN:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				char *playerName = new char[512];
				bitStream.Read(playerName);

				bool local = getGUID() == packet->guid;

				// Create player
				Player *player = new Player(m_game->getInputManager(), m_game->getWorld(), local);
				//m_game->getWorld()->addEntity(player);

				// If player is hosting locally
				if(local)
				{
					m_game->getGameOverlay()->setPlayer(player);
					m_game->getWorld()->getCamera()->setTargetEntity(player);
					m_game->getWorld()->m_localPlayer = player;
				}

				// Add to network objects
				m_players[playerName] = player;

				string playerFilePath = m_game->getWorld()->getWorldPath() + "/Players/" + playerName + ".obj";
				if(util::fileExists(playerFilePath))
				{
					LOG("Loading player '%s'...", playerName);

					// Load player data
					FileReader playerSaveFile(playerFilePath);
					player->loadSaveData(playerSaveFile);
					playerSaveFile.close();
				}
				else
				{
					LOG("Creating new player '%s'...", playerName);

					// Give default load out
					player->setPosition(Vector2F(0, 0));
					player->getStorage()->addItem(ITEM_PICKAXE_IRON);
					player->getStorage()->addItem(ITEM_AXE_IRON);
					player->getStorage()->addItem(ITEM_BOW_WOODEN);
					player->getStorage()->addItem(ITEM_CRAFTING_BENCH);
					player->getStorage()->addItem(ITEM_TORCH, 255);
					player->getStorage()->addItem(ITEM_ARROW, 255);
				}

				{
					// Brodcast the packet to all clients with the network id of the object added
					RakNet::BitStream bitStream(packet->data, packet->length, true);
					bitStream.Write(player->GetNetworkID());
					bitStream.Write(packet->guid);
					sendPacket(&bitStream);
				}
			}
			break;

			case ID_PLAYER_LEAVE:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				char *playerName = new char[512];
				bitStream.Read(playerName);

				savePlayer(playerName);

				delete m_players[playerName];
				m_players.erase(playerName);
			}
			break;

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
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				// Get entity
				EntityID entity;
				bitStream.Read(entity);

				NetworkObject *netObj;

				switch(entity)
				{
					case ENTITY_ZOMBIE:
					{
						Zombie *zombie = new Zombie(m_game->getWorld());
						zombie->setPosition(m_players.begin()->second->getPosition());
						netObj = zombie;
					}
				}

				{
					// Brodcast the packet to all clients with the network id of the object added
					RakNet::BitStream bitStream(packet->data, packet->length, true);
					bitStream.Write(netObj->GetNetworkID());
					bitStream.Write(packet->guid);
					sendPacket(&bitStream);
				}
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

void Server::save()
{
	LOG("Saving server...");

	m_game->getWorld()->getTerrain()->getChunkManager()->clear();

	// Save all players
	for(map<string, Pawn*>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr)
	{
		savePlayer(itr->first);
	}

	/*for(Entity *e : m_entities)
	{
	FileWriter file(m_worldPath + "/Objects");
	file << e->getID();
	e->createSaveData(file);
	file.close();
	}*/
}

void Server::savePlayer(string playerName)
{
	Pawn *player = m_players[playerName];
	string playerFilePath = m_game->getWorld()->getWorldPath() + "/Players/" + playerName + ".obj";
	FileWriter file(playerFilePath);
	player->createSaveData(file);
	file.close();
}