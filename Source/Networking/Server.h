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
	bool isServer() const { return true; }

	static Server *getInstance() { return s_instance; }

private:
	RakNet::RakPeerInterface *m_rakPeer;

	static Server *s_instance;
};

#endif // SERVER_H