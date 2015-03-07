#ifndef CLIENT_H
#define CLIENT_H

#include "Config.h"
#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class World;

class Client : public Connection
{
public:
	Client(World &world, const string &ip, const ushort port);

	void update();
	void sendPacket(RakNet::BitStream *bitStream);

private:
	World &m_world;
};

#endif // CLIENT_H