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
	m_world->onTick(e);

	// Send corrections for server-side objects to clients
	for(Entity *object : m_networkEntities)
	{
		RakNet::BitStream bitStream;
		bitStream.Write((RakNet::MessageID) ID_NETWORK_OBJECT_UPDATE);
		bitStream.Write(object->GetNetworkID());
		bitStream.Write(object->getData()->getID());
		object->packData(&bitStream);
		assert(m_rakPeer->Send(&bitStream, LOW_PRIORITY, RELIABLE_ORDERED, 0, object->getOriginGUID(), true) != 0);
	}

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

				// Create player
				Json::Value attributes;
				attributes["name"] = playerName;

				Player *player = createEntity<Player>(attributes);
				player->setOriginGUID(packet->guid);

				// Create player controller
				PlayerController *playerController = new PlayerController(m_game, false);
				player->setController(playerController);

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
					player->getStorage()->addItem(ItemData::GetByName("IronPickaxe")->getID());
					//player->getStorage()->addItem(ItemData::GetByName("iron_axe_item")->getID());
					player->getStorage()->addItem(ItemData::GetByName("Bow")->getID());
					//player->getStorage()->addItem(ItemData::GetByName("crafting_bench_item")->getID());
					//player->getStorage()->addItem(ItemData::GetByName("torch_item")->getID(), 255);
					player->getStorage()->addItem(ItemData::GetByName("Arrow")->getID(), 255);
				}

				{
					// Brodcast the packet to all clients with the network id of the object added
					RakNet::BitStream bitStream(packet->data, packet->length, true);
					bitStream.Write(packet->guid);
					bitStream.Write(player->GetNetworkID());
					player->packData(&bitStream);
					assert(m_rakPeer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true) != 0);

					// Send players which are already on the server to the newly joined client
					for(auto player : m_players)
					{
						RakNet::BitStream bitStream;
						bitStream.Write((RakNet::MessageID) ID_PLAYER_JOIN);
						bitStream.Write(player.second->getName().c_str());
						bitStream.Write(player.first);
						bitStream.Write(player.second->GetNetworkID());
						player.second->packData(&bitStream);
						assert(m_rakPeer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false) != 0);
					}

					{
						// Brodcast the packet to all clients with the network id of the object added
						RakNet::BitStream bitStream(packet->data, packet->length, true);
						bitStream.Write(packet->guid);
						bitStream.Write(player->GetNetworkID());
						player->packData(&bitStream);
						Storage *storage = player->getStorage();
						bitStream.Write(storage->getSize());
						for(int i = 0; i < storage->getSize(); i++)
						{
							Storage::Slot *slot = storage->getSlotAt(i);
							bitStream.Write(slot->getItem());
							bitStream.Write(slot->getAmount());
						}
						assert(m_rakPeer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, false) != 0);
					}

					// Map player GUID to player object
					m_players[packet->guid] = player;
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
					RakNet::BitStream bitStream(packet->data, packet->length, true);
					bitStream.WriteAlignedBytes((uchar*)chunk->m_blocks, CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT * sizeof(BlockID));
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

				//delete m_players[packet->guid];
				m_players.erase(packet->guid);
			}
			break;

			case ID_CREATE_ENTITY:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				RakNet::NetworkID networkID; bitStream.Read(networkID);
				EntityID entityID; bitStream.Read(entityID);
				RakNet::RakNetGUID guid; bitStream.Read(guid);

				LOG("Creating server object");
				Entity *entity = createEntityByID(entityID, networkID);
				entity->setOriginGUID(guid);
				entity->unpackData(&bitStream, true);

				// Send packet to all clients except the origin
				RakNet::BitStream outStream(packet->data, packet->length, false);
				assert(m_rakPeer->Send(&outStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true) != 0);
			}
			break;

			case ID_SET_BLOCK:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				int x; bitStream.Read(x);
				int y; bitStream.Read(y);
				WorldLayer layer; bitStream.Read(layer);
				BlockID blockID; bitStream.Read(blockID);

				m_world->getTerrain()->getChunkManager()->getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF), true)->setBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer, blockID);

				// Broadcast to other clients
				{
					RakNet::BitStream outStream(packet->data, packet->length, false);
					assert(m_rakPeer->Send(&outStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true) != 0);
				}
			}
			break;

			// Client want to correct the server
			case ID_NETWORK_OBJECT_UPDATE:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				// Find network object by network id
				RakNet::NetworkID networkID; bitStream.Read(networkID);
				Entity *object = m_networkIDManager.GET_OBJECT_FROM_ID<Entity*>(networkID);

				// Check if the object exists on the server
				if(object)
				{
					// Unpack data
					bitStream.IgnoreBytes(sizeof(EntityID));
					if(object->unpackData(&bitStream, false))
					{
						// Server accepted the packet, send server-object state to all clients except the origin
						RakNet::BitStream bitStream(packet->data, packet->length, false);
						assert(m_rakPeer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true) != 0);
					}
					else
					{
						// The server did not accept the client packet, send server-object state to all clients
						RakNet::BitStream bitStream;
						bitStream.Write((RakNet::MessageID) ID_NETWORK_OBJECT_UPDATE);
						bitStream.Write(object->GetNetworkID());
						object->packData(&bitStream);
						assert(m_rakPeer->Send(&bitStream, LOW_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
					}
				}
			}
			break;

			case ID_SEND_INPUT_STATE:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				uint inputState; bitStream.Read(inputState);
				Controller *controller = m_players[packet->guid]->getController();
				controller->m_inputState = inputState;
				bitStream.Read(controller->m_inputPosition.x);
				bitStream.Read(controller->m_inputPosition.y);

				{
					RakNet::BitStream outStream;
					outStream.Write((RakNet::MessageID)ID_SEND_INPUT_STATE);
					outStream.Write(packet->guid);
					outStream.Write(inputState);
					outStream.Write(controller->m_inputPosition.x);
					outStream.Write(controller->m_inputPosition.y);
					assert(m_rakPeer->Send(&outStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true) != 0);
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