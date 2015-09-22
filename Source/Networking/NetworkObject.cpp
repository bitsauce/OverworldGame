#include "NetworkObject.h"
#include "Connection.h"
#include "Server.h"
#include "Client.h"

NetworkObject::NetworkObject()
{
	Connection::getInstance()->addNetworkObject(this);
}

NetworkObject::~NetworkObject()
{
	Connection::getInstance()->removeNetworkObject(this);
}