#include "NetworkObject.h"
#include "Connection.h"

NetworkObject::NetworkObject()
{
	Connection::getInstance()->addNetworkObject(this);
}

NetworkObject::~NetworkObject()
{
}