#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H

#include "NetworkIDObject.h"
#include "BitStream.h"
//>REMOVE ME<
class Connection;

class NetworkObject : public RakNet::NetworkIDObject
{
public:
	NetworkObject();
	~NetworkObject();
	
	virtual void pack(RakNet::BitStream *bitStream, const Connection *conn) = 0;
	virtual void unpack(RakNet::BitStream *bitStream, const Connection *conn) = 0;

private:
};

#endif // NETWORK_OBJECT_H