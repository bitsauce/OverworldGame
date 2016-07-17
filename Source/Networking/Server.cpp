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
#include "Items/ItemData.h"

Server::Server(OverworldGame *game, const ushort port) :
	Connection(true),
	m_game(game)
{
	RakNet::SocketDescriptor socketDescriptor(port, 0);
	if(m_rakPeer->Startup(2, &socketDescriptor, 1) != RakNet::RAKNET_STARTED)
	{
		LOG("Could not host server on port '%i' (port may be in use), hosting local server on port 0 instead", port);
		socketDescriptor.port = 0;
		assert(m_rakPeer->Startup(2, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);
	}
	m_rakPeer->SetMaximumIncomingConnections(2);

	m_rakPeer->SetTimeoutTime(600000, RakNet::UNASSIGNED_SYSTEM_ADDRESS); // For debugging
}

#include "Entities/EntityData.h"

void Server::onTick(TickEvent *e)
{
	m_isServer = false;
	for(NetworkObject *object : m_networkObjects)
	{
		if(object->m_local)
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

				// Send entities???
				for(Entity *entity : m_networkEntities)
				{
					RakNet::BitStream bitStream;
					bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
					bitStream.Write(entity->getData()->getID());
					bitStream.Write(entity->GetNetworkID());
					bitStream.Write(m_rakPeer->GetMyGUID()/*entity->GetGUID()*/);
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
				Player *player = new Player(playerName, m_game, local);

				// Create player controller
				PlayerController *playerController = new PlayerController(m_game, local);
				playerController->SetNetworkIDManager(&m_networkIDManager);
				player->setController(playerController);
				player->m_local = playerController->m_local = local;

				// If player is hosting locally
				if(local)
				{
					m_game->getGameOverlay()->setPlayer(player);
					m_game->getWorld()->getCamera()->setTargetEntity(player);
					m_game->getWorld()->m_localPlayer = player;
				}

				// Add to network objects
				m_players[packet->guid] = player;

				m_networkEntities.push_back(player);

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

					// Find spawn location
					{
						const int x = rand() % 1024 - 512;
						int y = 0;
						while(!m_game->getWorld()->getTerrain()->isBlockAt(x, y, WORLD_LAYER_MIDDLE)) y++;
						player->setPosition(x * BLOCK_PXF, y * BLOCK_PXF - player->getHeight());
					}

					// Give default load out
					player->getStorage()->addItem(ItemData::getByName("iron_pickaxe_item")->getID());
					player->getStorage()->addItem(ItemData::getByName("iron_axe_item")->getID());
					player->getStorage()->addItem(ItemData::getByName("bow_item")->getID());
					player->getStorage()->addItem(ItemData::getByName("crafting_bench_item")->getID());
					player->getStorage()->addItem(ItemData::getByName("torch_item")->getID(), 255);
					player->getStorage()->addItem(ItemData::getByName("arrow_item")->getID(), 255);
				}

				{
					// Brodcast the packet to all clients with the network id of the object added
					RakNet::BitStream bitStream(packet->data, packet->length, true);
					bitStream.Write(packet->guid);
					bitStream.Write(player->GetNetworkID());
					bitStream.Write(playerController->GetNetworkID());
					player->pack(&bitStream, this);
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

				savePlayer(m_players[packet->guid]);

				delete m_players[packet->guid];
				m_players.erase(packet->guid);
			}
			break;

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

					RakNet::BitStream outStream;
					outStream.Write((RakNet::MessageID)ID_NETWORK_OBJECT_UPDATE);
					outStream.Write(object->GetNetworkID());
					object->pack(&outStream, this);
					
					// Send packet to all except the one who sent it
					assert(m_rakPeer->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true) != 0);
					//sendPacket(&outStream);
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
	for(map<RakNet::RakNetGUID, Player*>::iterator itr = m_players.begin(); itr != m_players.end(); ++itr)
	{
		savePlayer(itr->second);
	}

	// TODO: Save all entities to files
	/*for(Entity *e : m_entities)
	{
	FileWriter file(m_worldPath + "/Objects");
	file << e->getID();
	e->createSaveData(file);
	file.close();
	}*/
}

void Server::savePlayer(Player *player)
{
	string playerFilePath = m_game->getWorld()->getWorldPath() + "/Players/" + player->getName() + ".obj";
	FileWriter file(playerFilePath);
	player->createSaveData(file);
	file.close();
}