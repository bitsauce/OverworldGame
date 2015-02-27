#ifndef SERVER_H
#define SERVER_H

#include "Game/GameObject.h"

namespace RakNet {
	class RakPeerInterface;
}

class Server : public GameObject
{
public:
	Server(const ushort port);

	void update();

private:
	RakNet::RakPeerInterface *m_rakPeer;
};

#endif // SERVER_H