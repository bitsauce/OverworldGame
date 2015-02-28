#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H

#include "NetworkIDObject.h"
#include "BitStream.h"

class NetworkObject : RakNet::NetworkIDObject
{
public:
	NetworkObject();
	~NetworkObject();
	
	virtual void pack(RakNet::BitStream *bitStream);
	virtual void unpack(RakNet::BitStream *bitStream);

private:
};

#endif // NETWORK_OBJECT_H