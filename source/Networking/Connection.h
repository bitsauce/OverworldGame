#ifndef CONNECTION_H
#define CONNECTION_H

#include "Config.h"

class NetworkObject;

class Connection
{
public:
	virtual void connect() = 0;
	virtual bool isServer() const = 0;
	void addNetworkObject(NetworkObject *object) { m_networkObjects.push_back(object); }
	static Connection *getInstance() { return s_instance; }

private:
	static void setInstance(Connection *instance) { s_instance = instance; }
	static Connection *s_instance;
	list<NetworkObject*> m_networkObjects;
};

#endif // CONNECTION_H