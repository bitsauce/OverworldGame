#include "NetworkObject.h"
#include "Connection.h"
#include "Server.h"
#include "Client.h"

NetworkObject::NetworkObject() :
	m_local(false)
{
	Connection::getInstance()->addNetworkObject(this);
}

NetworkObject::~NetworkObject()
{
	Connection::getInstance()->removeNetworkObject(this);
}

/*bool NetworkObject::isLocal() const
{
	return m_local;
}*/