#include "NetworkObject.h"
#include "Connection.h"
#include "Server.h"
#include "Client.h"

NetworkObject::NetworkObject(Connection *conn) :
	m_local(false),
	m_connection(conn)
{
	m_connection->addNetworkObject(this);
}

NetworkObject::~NetworkObject()
{
	m_connection->removeNetworkObject(this);
}

/*bool NetworkObject::isLocal() const
{
	return m_local;
}*/