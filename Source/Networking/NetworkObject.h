#pragma once

#include "NetworkIDObject.h"
#include "BitStream.h"

class Connection;

class NetworkObject : public RakNet::NetworkIDObject
{
public:
	NetworkObject(Connection *conn);

	virtual void packData(RakNet::BitStream *bitStream, const Connection *conn) { }
	virtual bool unpackData(RakNet::BitStream *bitStream, const Connection *conn) { return true; }

	RakNet::RakNetGUID getOriginGUID() const
	{
		return m_originGUID;
	}

	void setOriginGUID(RakNet::RakNetGUID originGUID);

	bool isClientObject() const
	{
		return m_isClientObject;
	}

protected:
	Connection *m_connection;

private:
	RakNet::RakNetGUID m_originGUID;
	bool m_isClientObject;
};
