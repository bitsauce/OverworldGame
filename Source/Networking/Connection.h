#ifndef CONNECTION_H
#define CONNECTION_H

#include "Config.h"
#include "NetworkIDManager.h"
#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "PacketPriority.h"
#include "BitStream.h"
#include "NetworkIDObject.h"
#include "NetworkIDManager.h"
#include "PacketLogger.h"
#include "Entities/EntityData.h"
#include "World/World.h"

class Entity;

class Connection : public SceneObject
{
	friend class Entity;
public:
	Connection(const bool isServer) :
		m_isServer(isServer),
		m_world(0)
	{
		m_rakPeer = RakNet::RakPeerInterface::GetInstance(); 
	}

	bool isServer() const { return m_isServer; }
	bool isClient() const { return !m_isServer; }
	RakNet::RakNetGUID getGUID() const { return m_rakPeer->GetMyGUID(); }
	RakNet::RakPeerInterface *getRakPeer() const { return m_rakPeer; }

	template<typename T> T *createEntity(const Json::Value &attributes, RakNet::NetworkID networkID = RakNet::UNASSIGNED_NETWORK_ID)
	{
		T *entity = m_world->createEntity<T>(attributes);
		m_networkEntities.push_back(entity);
		entity->SetNetworkIDManager(&m_networkIDManager);
		if(networkID == RakNet::UNASSIGNED_NETWORK_ID)
		{
			RakNet::BitStream bitStream;
			bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
			bitStream.Write(entity->GetNetworkID());
			bitStream.Write(entity->getData()->getID());
			bitStream.Write(m_rakPeer->GetMyGUID());
			entity->packData(&bitStream);
			assert(m_rakPeer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
		}
		else
		{
			entity->SetNetworkID(networkID);
		}
		return entity;
	}

	Entity *createEntityByID(const EntityID id, RakNet::NetworkID networkID = RakNet::UNASSIGNED_NETWORK_ID)
	{
		Entity *entity = m_world->createEntityByID(id);
		m_networkEntities.push_back(entity);
		entity->SetNetworkIDManager(&m_networkIDManager);
		if(networkID == RakNet::UNASSIGNED_NETWORK_ID)
		{
			RakNet::BitStream bitStream;
			bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
			bitStream.Write(entity->GetNetworkID());
			bitStream.Write(entity->getData()->getID());
			bitStream.Write(m_rakPeer->GetMyGUID());
			entity->packData(&bitStream);
			assert(m_rakPeer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
		}
		else
		{
			entity->SetNetworkID(networkID);
		}
		return entity;
	}

	void removeEntity(Entity *entity)
	{
		// TODO: Send entity removal packet to other clients
		// NOTE TO SELF: Maybe a problem with the current "create on
		// update of non-existent object" is that objects can be reconstructed after they're deleted.
		m_networkEntities.remove(entity);
		m_world->removeEntity(entity);
		delete entity;
	}

	void updateBlock(const int x, const int y, const WorldLayer layer, const BlockID blockID)
	{
		RakNet::BitStream bitStream;
		bitStream.Write((RakNet::MessageID)ID_SET_BLOCK);
		bitStream.Write(x);
		bitStream.Write(y);
		bitStream.Write(layer);
		bitStream.Write(blockID);
		assert(m_rakPeer->Send(&bitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true) != 0);
	}

	World *getWorld() const { return m_world; }

protected:
	bool m_isServer;
	RakNet::RakPeerInterface *m_rakPeer;
	RakNet::NetworkIDManager m_networkIDManager;
	World *m_world;
	list<Entity*> m_networkEntities;
};

#endif // CONNECTION_H