#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H

#include "NetworkIDObject.h"
#include "BitStream.h"

class NetworkObject : public RakNet::NetworkIDObject
{
public:
	NetworkObject();
	~NetworkObject();
	
	virtual void pack(RakNet::BitStream *bitStream) = 0;
	virtual void unpack(RakNet::BitStream *bitStream) = 0;

private:
};

#endif // NETWORK_OBJECT_H