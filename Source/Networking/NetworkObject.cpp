#include "NetworkObject.h"
#include "Connection.h"
#include "Server.h"
#include "Client.h"

NetworkObject::NetworkObject(Connection *conn) :
	m_connection(conn)
{
}

void NetworkObject::setOriginGUID(RakNet::RakNetGUID originGUID)
{
	m_originGUID = originGUID;
	m_isClientObject = m_connection->getGUID() == m_originGUID;
}
