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

class NetworkObject;
class Entity;
class World;

class Connection : public SceneObject
{
	friend class NetworkObject;
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

	World *getWorld() const { return m_world; }

	virtual void sendPacket(RakNet::BitStream *bitStream) { LOG("SEND PACKET?????"); }

protected:
	bool m_isServer;
	RakNet::RakPeerInterface *m_rakPeer;
	RakNet::NetworkIDManager m_networkIDManager;
	list<NetworkObject*> m_networkObjects;
	list<Entity*> m_networkEntities;
	
	World *m_world;

protected:
	void addNetworkObject(NetworkObject *object);
	void removeNetworkObject(NetworkObject *object);
};

#endif // CONNECTION_H