#ifndef CLIENT_H
#define CLIENT_H

#include "Config.h"
#include "Game/GameObject.h"

namespace RakNet {
	class RakPeerInterface;
}

class Client : public GameObject
{
public:
	Client(const string &ip, const ushort port);

	void update();

private:
	RakNet::RakPeerInterface *m_rakPeer;
};

#endif // CLIENT_H