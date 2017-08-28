#include "Connection.h"
#include "NetworkObject.h"

void Connection::addNetworkObject(NetworkObject *object)
{
	object->SetNetworkIDManager(&m_networkIDManager);
	m_networkObjects.push_back(object);
}

void Connection::removeNetworkObject(NetworkObject *object)
{
	//object->SetNetworkIDManager(0);
	m_networkObjects.remove(object);
}