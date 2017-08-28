#ifndef NETWORK_OBJECT_H
#define NETWORK_OBJECT_H

#include "NetworkIDObject.h"
#include "BitStream.h"

class Connection;

class NetworkObject : public RakNet::NetworkIDObject
{
public:
	NetworkObject(Connection *conn);
	~NetworkObject();
	
	// TODO: pack -> onPack, unpack -> onUnpack
	virtual void pack(RakNet::BitStream *bitStream, const Connection *conn) {}
	virtual void unpack(RakNet::BitStream *bitStream, const Connection *conn) {}

	//bool isLocal() const;
	bool m_local;
private:
	Connection *m_connection;
	//RakNet::RakNetGUID m_origin;
};

#endif // NETWORK_OBJECT_H