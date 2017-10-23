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

#include "Entities/EntityData.h"
#include "Entities/Player.h"
#include "PlayerController.h"

#include "Gui/Gui.h"
#include "Gui/GameOverlay/GameOverlay.h"

Client::Client(Overworld *game) :
	Connection(false),
	m_game(game),
	m_joinFinalized(false),
	m_joinProgress(0.0f),
	m_serverGuid(RakNet::UNASSIGNED_RAKNET_GUID),
	m_serverAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS)
{
}

Client::ErrorCode Client::join(const string &playerName, const string &ip, const ushort port)
{
	// Setup connection
	LOG("Connecting to server %s:%i", ip.c_str(), port);

	RakNet::SocketDescriptor socketDescriptor;
	if(m_rakPeer->Startup(1, &socketDescriptor, 1) != RakNet::RAKNET_STARTED) return COULD_NOT_SETUP_SOCKET;
	if(m_rakPeer->Connect(ip.c_str(), port, 0, 0) != RakNet::CONNECTION_ATTEMPT_STARTED) return COULD_NOT_CONNECT;

	m_rakPeer->SetTimeoutTime(600000, RakNet::UNASSIGNED_SYSTEM_ADDRESS); // For debugging purposes

	m_playerName = playerName;

	// Load world from the server
	LOG("Receiving world data");
	m_world = new World(this);

	// Reset join progress
	ChunkManager *chunkManager = m_world->getTerrain()->getChunkManager();
	chunkManager->updateLoadingAndActiveArea(Vector2F(0.0f));
	ChunkManager::ChunkArea activeArea = chunkManager->getActiveArea();
	m_joinProgress = 0;
	m_joinNumSteps = 2 + activeArea.getWidth() * activeArea.getHeight();

	return SUCCESS;
}

void Client::onTick(TickEvent *e)
{
	// Send input state
	if(m_joinFinalized)
	{
		m_world->m_localPlayer->getController()->m_inputPosition = m_world->getCamera()->getInputPosition();

		RakNet::BitStream bitStream;
		bitStream.Write((RakNet::MessageID) ID_SEND_INPUT_STATE);
		bitStream.Write(m_world->m_localPlayer->getController()->m_inputState);
		bitStream.Write(m_world->m_localPlayer->getController()->m_inputPosition.x);
		bitStream.Write(m_world->m_localPlayer->getController()->m_inputPosition.y);
		sendPacket(&bitStream);
	}

	// Process incomming packets
	for(RakNet::Packet *packet = m_rakPeer->Receive(); packet; m_rakPeer->DeallocatePacket(packet), packet = m_rakPeer->Receive())
	{
		switch(packet->data[0])
		{
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				LOG("Connection request accepted by server %s with GUID %s", packet->systemAddress.ToString(true), packet->guid.ToString());

				// Attempting to join as client
				LOG("Attempting to join as '%s'", m_playerName.c_str());
				RakNet::BitStream bitStream;
				bitStream.Write((RakNet::MessageID) ID_PLAYER_JOIN);
				bitStream.Write(m_playerName.c_str());
				sendPacket(&bitStream);

				m_serverGuid = packet->guid;
				m_serverAddress = packet->systemAddress;

				m_joinProgress++;
			}
			break;

			case ID_PLAYER_JOIN:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				// Read packet data
				char *playerName = new char[MAX_USERNAME_LENGTH]; bitStream.Read(playerName);
				RakNet::RakNetGUID playerGUID; bitStream.Read(playerGUID);
				RakNet::NetworkID playerNetworkID; bitStream.Read(playerNetworkID);
				bool isLocalPlayer = playerGUID == getGUID();

				// Create player
				Json::Value attributes;
				attributes["name"] = playerName;

				Player *player = createEntity<Player>(attributes, playerNetworkID);
				player->setOriginGUID(playerGUID);
				player->unpackData(&bitStream);
				player->setPosition(player->getPosition()); // Make sure lastPosition == position

				// Create player controller
				PlayerController *playerController = new PlayerController(m_game, isLocalPlayer);
				player->setController(playerController);

				if(isLocalPlayer)
				{
					uint storageSize; bitStream.Read(storageSize);
					for(int i = 0; i < storageSize; i++)
					{
						ItemID item; bitStream.Read(item);
						int amount;  bitStream.Read(amount);
						player->getStorage()->addItem(item, amount);
					}


					m_world->getCamera()->setTargetEntity(player);
					m_world->m_localPlayer = player;

					// Request chunks
					ChunkManager *chunkManager = m_world->getTerrain()->getChunkManager();
					chunkManager->updateLoadingAndActiveArea(player->getPosition());
					ChunkManager::ChunkArea activeArea = chunkManager->getActiveArea();
					for(int y = activeArea.y0; y <= activeArea.y1; y++)
					{
						for(int x = activeArea.x0; x <= activeArea.x1; x++)
						{
							chunkManager->getChunkAt(x, y, true);
						}
					}
					LOG("Requested %i chunks", activeArea.getWidth() * activeArea.getHeight());

					{
						RakNet::BitStream bitStream;
						bitStream.Write((RakNet::MessageID) ID_PLAYER_JOIN_FINALIZE);
						sendPacket(&bitStream);
					}

					m_joinProgress++;
				}

				m_players[playerGUID] = player;
			}
			break;

			case ID_REQUEST_CHUNK:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				// Read chunk data
				int chunkX, chunkY;
				bitStream.Read(chunkX);
				bitStream.Read(chunkY);

				BlockID blocks[CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT];
				bitStream.ReadAlignedBytes((uchar*)&blocks, CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT * sizeof(BlockID));

				// Load chunk
				m_world->getTerrain()->getChunkManager()->getChunkAt(chunkX, chunkY, false)->load(chunkX, chunkY, blocks);

				LOG("Chunk [%i, %i] received", chunkX, chunkY);

				m_joinProgress++;
			}
			break;

			case ID_PLAYER_JOIN_FINALIZE:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				RakNet::RakNetGUID guid;
				bitStream.Read(guid);

				if(guid == getGUID())
				{
					m_joinFinalized = true;
					LOG("Join finalized!");
				}
			}
			break;

			case ID_CREATE_ENTITY:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				RakNet::NetworkID networkID; bitStream.Read(networkID);
				EntityID entityID; bitStream.Read(entityID);
				RakNet::RakNetGUID guid; bitStream.Read(guid);

				if(entityID != 1) // TODO: Try to fix. Shouldn't receive update packets from players before we're ready maybe?
				{
					Entity *entity = createEntityByID(entityID, networkID);
					entity->setOriginGUID(guid);
					entity->unpackData(&bitStream);

					LOG("Creating client object");
				}
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
			}
			break;

			// Apply server correction
			case ID_NETWORK_OBJECT_UPDATE:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));

				RakNet::NetworkID networkID; bitStream.Read(networkID);
				Entity *object = m_networkIDManager.GET_OBJECT_FROM_ID<Entity*>(networkID);
				if(object)
				{
					object->unpackData(&bitStream);
				}
			}
			break;

			case ID_SEND_INPUT_STATE:
			{
				if(!m_joinFinalized) break;

				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				RakNet::RakNetGUID playerGuid; bitStream.Read(playerGuid);

				if(m_players.find(playerGuid) != m_players.end())
				{
					Controller *controller = m_players[playerGuid]->getController();
					uint inputState; bitStream.Read(inputState);
					controller->m_inputState = inputState;
					bitStream.Read(controller->m_inputPosition.x);
					bitStream.Read(controller->m_inputPosition.y);
				}
			}
			break;

			case ID_CHAT_MESSAGE:
			{
				RakNet::BitStream bitStream(packet->data, packet->length, false);
				bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
				uint size; bitStream.Read(size);
				string message; message.resize(size);
				bitStream.Read(&message[0]);
				m_game->getGameOverlay()->getChat()->insertMessage(message);
			}
			break;

			default:
				LOG("Received unknown packet type %s", RakNet::PacketLogger::BaseIDTOString(packet->data[0]));
				break;
		}
	}

	if(++m_tickCounter >= m_tickRate)
	{
		// Send corrections of client-side objects to server
		for(Entity *object : m_networkEntities)
		{
			if(object->isClientObject())
			{
				RakNet::BitStream bitStream;
				bitStream.Write((RakNet::MessageID) ID_NETWORK_OBJECT_UPDATE);
				bitStream.Write(object->GetNetworkID());
				object->packData(&bitStream);
				sendPacket(&bitStream);
			}
		}
		m_tickCounter = 0;
	}

	if(pushingPastCapacity())
	{
		m_tickRate++;
		m_increasePacketRateCounter = 0;
		m_ticksBeforeIncreasePacketRate = max(m_ticksBeforeIncreasePacketRate + 1, 100);
		LOG("Using too much bandwidth, reducing packet rate");
	}

	if(m_increasePacketRateCounter++ == m_ticksBeforeIncreasePacketRate)
	{
		if(m_tickRate > 1) m_tickRate--;
		LOG("Trying to increase packet rate");
	}
}

// Returns true if RakNet's output queue is increasing over time
bool Client::pushingPastCapacity()
{
	RakNet::Time currentTime = RakNet::GetTime();
	static double bufferedBytesLastTick = 0.0;
	static RakNet::Time whenStartedPushingPastCapacity = currentTime;
	RakNet::RakNetStatistics rns;
	m_rakPeer->GetStatistics(m_serverAddress, &rns);
	double bufferedBytesThisTick = rns.bytesInSendBuffer[IMMEDIATE_PRIORITY] + rns.bytesInSendBuffer[HIGH_PRIORITY] + rns.bytesInSendBuffer[MEDIUM_PRIORITY] + rns.bytesInSendBuffer[LOW_PRIORITY];
	if(bufferedBytesThisTick > bufferedBytesLastTick)
	{
		if(currentTime - whenStartedPushingPastCapacity > 500)
		{
			bufferedBytesLastTick = bufferedBytesThisTick;
			return true;
		}
	}
	else
	{
		whenStartedPushingPastCapacity = currentTime;
	}
	bufferedBytesLastTick = bufferedBytesThisTick;
	return false;
}

void Client::sendPacket(RakNet::BitStream *bitStream)
{
	assert(m_rakPeer->Send(bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
}