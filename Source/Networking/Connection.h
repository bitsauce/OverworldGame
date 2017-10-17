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
		if(networkID != RakNet::UNASSIGNED_NETWORK_ID) entity->SetNetworkID(networkID);
		return entity;
	}

	Entity *createEntityByID(const EntityID id, RakNet::NetworkID networkID = RakNet::UNASSIGNED_NETWORK_ID)
	{
		Entity *entity = m_world->createEntityByID(id);
		m_networkEntities.push_back(entity);
		entity->SetNetworkIDManager(&m_networkIDManager);
		if(networkID != RakNet::UNASSIGNED_NETWORK_ID) entity->SetNetworkID(networkID);
		return entity;
	}

	World *getWorld() const { return m_world; }

	virtual void sendPacket(RakNet::BitStream *bitStream) { LOG("SEND PACKET?????"); }

protected:
	bool m_isServer;
	RakNet::RakPeerInterface *m_rakPeer;
	RakNet::NetworkIDManager m_networkIDManager;
	World *m_world;
	list<Entity*> m_networkEntities;
};

#endif // CONNECTION_H