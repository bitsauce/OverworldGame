#ifndef SERVER_H
#define SERVER_H

#include "Game/GameObject.h"
#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class Server : public GameObject, public Connection
{
public:
	Server(const ushort port);

	void update();
	void sendPacket(RakNet::BitStream *bitStream);

	RakNet::RakPeerInterface *getRakPeer() const { return m_rakPeer; }
};

#endif // SERVER_H