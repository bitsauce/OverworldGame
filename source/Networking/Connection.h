#ifndef CONNECTION_H
#define CONNECTION_H

#include "Config.h"
#include "NetworkIDManager.h"

class NetworkObject;

class Connection
{
public:
	void addNetworkObject(NetworkObject *object) { s_networkObjects.push_back(object); }
	void removeNetworkObject(NetworkObject *object) { s_networkObjects.remove(object); }
	static RakNet::NetworkIDManager *getNetworkIDManager() { return &s_networkIDManager; }

protected:
	static RakNet::NetworkIDManager s_networkIDManager;
	static list<NetworkObject*> s_networkObjects;
};

#endif // CONNECTION_H