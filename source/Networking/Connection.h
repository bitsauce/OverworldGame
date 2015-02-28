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

class Connection
{
public:
	Connection(const bool isServer) : m_isServer(isServer), m_rakPeer(nullptr) { s_instance = this; }

	bool isServer() const { return m_isServer; }
	bool isClient() const { return !m_isServer; }
	RakNet::RakNetGUID getGUID() const { return m_rakPeer->GetMyGUID(); }

	//void addNetworkObject(NetworkObject *object) { s_networkObjects.push_back(object); }
	//void removeNetworkObject(NetworkObject *object) { s_networkObjects.remove(object); }

	static const Connection *getInstance() { return s_instance; }

protected:
	bool m_isServer;
	RakNet::RakPeerInterface *m_rakPeer;
	RakNet::NetworkIDManager m_networkIDManager;
	list<NetworkObject*> m_networkObjects;
	static Connection *s_instance;
};

#endif // CONNECTION_H