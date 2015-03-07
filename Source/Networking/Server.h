#ifndef SERVER_H
#define SERVER_H

#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class World;

class Server : public Connection
{
public:
	Server(World &world, const ushort port);

	void update();
	void sendPacket(RakNet::BitStream *bitStream);

	RakNet::RakPeerInterface *getRakPeer() const { return m_rakPeer; }

private:
	World &m_world;
};

#endif // SERVER_H