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
#include "PlayerController.h"
#include "World/World.h"
#include "Entities/EntityData.h"
#include "Entities/Pawn.h"
#include "Entities/Player.h"
#include "Entities/AIController.h"
#include "Entities/Mobs/Zombie.h"
#include "Game/Game.h"
#include "Gui/GameOverlay/GameOverlay.h"
#include "Items/ItemData.h"

Server::Server(Overworld *game) :
	Connection(true),
	m_game(game)
{
}

Server::ErrorCode Server::host(const string &worldName, const ushort port)
{
	// Setup sockets
	LOG("Attempting to host server on port %i...", port);

	RakNet::SocketDescriptor socketDescriptor(port, 0);
	if(m_rakPeer->Startup(2, &socketDescriptor, 1) != RakNet::RAKNET_STARTED)
	{
		LOG("Could not host server on port %i (port may be in use). Hosting local server on port 0 instead", port);
		socketDescriptor.port = 0;
		if(m_rakPeer->Startup(2, &socketDescriptor, 1) != RakNet::RAKNET_STARTED) return COULD_NOT_SETUP_SOCKET;
	}
	m_rakPeer->SetMaximumIncomingConnections(2);

	LOG("Server was successfully hosted");
	m_rakPeer->SetTimeoutTime(600000, RakNet::UNASSIGNED_SYSTEM_ADDRESS); // For debugging purposes

	// Create and load world
	LOG("Loading world '%s'", worldName.c_str());
	m_world = new World(this);
	if(!m_world->load(worldName))
	{
		LOG("World '%s' does not exist. Creating world", worldName.c_str());
		m_world->create(worldName);
	}

	return SUCCESS;
}

void Server::onTick(TickEvent *e)
{
	/*m_isServer = false;
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
	m_isServer = true;*/

	for(RakNet::Packet *packet = m_rakPeer->Receive(); packet; m_rakPeer->DeallocatePacket(packet), packet = m_rakPeer->Receive())
	{
		switch(packet->data[0])
		{
			case ID_NEW_INCOMING_CONNECTION:
			{
				LOG("Client connected from %s with GUID %s", packet->systemAddress.ToString(true), packet->guid.ToString());
			}
			break;

			case ID_PLAYER_JOIN:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				char *playerName = new char[MAX_USERNAME_LENGTH];
				bitStream.Read(playerName);

				LOG("Player '%s' is joining...", playerName);

				// If the GUID of the client sending the packet is the same as the server,
				// the client is playing on a server locally
				bool local = getGUID() == packet->guid;

				// Create player
				Json::Value attributes;
				attributes["name"] = playerName;
				Player *player = new Player(m_world, attributes);

				// Create player controller
				PlayerController *playerController = new PlayerController(m_game, local);
				playerController->SetNetworkIDManager(&m_networkIDManager);
				player->setController(playerController);
				player->m_local = playerController->m_local = local;

				// If player is hosting locally
				if(local)
				{
					m_game->getGameOverlay()->setPlayer(player);
					m_world->getCamera()->setTargetEntity(player);
					m_world->m_localPlayer = player;
				}

				// Add to network objects
				m_players[packet->guid] = player;

				m_networkEntities.push_back(player);

				string playerFilePath = m_world->getWorldPath() + "/Players/" + playerName + ".obj";
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
						while(!m_world->getTerrain()->isBlockAt(x, y, WORLD_LAYER_MIDDLE)) y++;
						player->setPosition(x * BLOCK_PXF, y * BLOCK_PXF - player->getHeight());
					}

					// Give default load out
					player->getStorage()->addItem(ItemData::getByName("iron_pickaxe_item")->getID());
					player->getStorage()->addItem(ItemData::getByName("iron_axe_item")->getID());
					player->getStorage()->addItem(ItemData::getByName("bow_item")->getID());
					//player->getStorage()->addItem(ItemData::getByName("crafting_bench_item")->getID());
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
					assert(m_rakPeer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
				}
			}
			break;

			case ID_PLAYER_JOIN_FINALIZE:
			{
				// Inform all players that a new player has finalized joining
				RakNet::BitStream bitStream(packet->data, packet->length, true);
				bitStream.Write(packet->guid);
				assert(m_rakPeer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
			}
			break;

			case ID_REQUEST_CHUNK:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				int chunkX, chunkY;
				bitStream.Read(chunkX);
				bitStream.Read(chunkY);
				Chunk *chunk = m_world->getTerrain()->getChunkManager()->getChunkAt(chunkX, chunkY, true);

				{
					// OKAY. The problem is sending pointers. Let's not.
					// TODO: Rewrite the chunks to use raw integer values and send those instead
					// (or create a serialize function, for example)
					RakNet::BitStream bitStream(packet->data, packet->length, true);
					bitStream.WriteAlignedBytes((uchar*)chunk->m_blocks, CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT * sizeof(Block));
					assert(m_rakPeer->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false) != 0);
				}
			}
			break;

			case ID_PLAYER_LEAVE:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				char *playerName = new char[MAX_USERNAME_LENGTH];
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
				m_world->getTerrain()->setBlockAt(x, y, layer, BlockData::get(blockID), true);
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
					/*case ENTITY_ZOMBIE:
					{
						Zombie *zombie = new Zombie(m_world);
						zombie->setPosition(m_players.begin()->second->getPosition());
						netObj = zombie;
					}*/
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

void Server::save()
{
	LOG("Saving server...");

	if(m_world) m_world->save();

	m_world->getTerrain()->getChunkManager()->clear();

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
	string playerFilePath = m_world->getWorldPath() + "/Players/" + player->getName() + ".obj";
	FileWriter file(playerFilePath);
	player->createSaveData(file);
	file.close();
}