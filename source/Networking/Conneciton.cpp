#include "Connection.h"

RakNet::NetworkIDManager Connection::s_networkIDManager;
list<NetworkObject*> Connection::s_networkObjects;