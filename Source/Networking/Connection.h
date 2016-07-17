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

class Connection : public SceneObject
{
	friend class NetworkObject;
public:
	Connection(const bool isServer) :
		m_isServer(isServer)
	{
		m_rakPeer = RakNet::RakPeerInterface::GetInstance(); 
		s_instance = this;
	}

	bool isServer() const { return m_isServer; }
	bool isClient() const { return !m_isServer; }
	RakNet::RakNetGUID getGUID() const { return m_rakPeer->GetMyGUID(); }
	RakNet::RakPeerInterface *getRakPeer() const { return m_rakPeer; }

	static Connection *getInstance() { return s_instance; }

protected:
	bool m_isServer;
	RakNet::RakPeerInterface *m_rakPeer;
	RakNet::NetworkIDManager m_networkIDManager;
	list<NetworkObject*> m_networkObjects;
	list<Entity*> m_networkEntities;
	static Connection *s_instance;

protected:
	void addNetworkObject(NetworkObject *object);
	void removeNetworkObject(NetworkObject *object);
};

#endif // CONNECTION_H