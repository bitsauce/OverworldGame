#ifndef CLIENT_H
#define CLIENT_H

#include "Config.h"
#include "Game/GameObject.h"
#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class Client : public GameObject, public Connection
{
public:
	Client(const string &ip, const ushort port);

	void update();
	void sendPacket(RakNet::BitStream *bitStream);
};

#endif // CLIENT_H