#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H

#include "NetworkIDObject.h"
#include "BitStream.h"

class Connection;

class NetworkObject : public RakNet::NetworkIDObject
{
	friend class Server;
	friend class Client;
public:
	virtual void packData(RakNet::BitStream *bitStream, const Connection *conn) {}
	virtual bool unpackData(RakNet::BitStream *bitStream, const Connection *conn) { return true; }

	RakNet::RakNetGUID getOriginGUID() const
	{
		return m_originGUID;
	}

private:
	RakNet::RakNetGUID m_originGUID;
};

#endif // NETWORK_OBJECT_H