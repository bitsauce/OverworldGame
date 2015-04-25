#ifndef SERVER_H
#define SERVER_H

#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class Game;

class Server : public Connection
{
public:
	Server(Game *game, const ushort port);

	void update();
	void sendPacket(RakNet::BitStream *bitStream);

	RakNet::RakPeerInterface *getRakPeer() const { return m_rakPeer; }

private:
	Game *m_game;
};

#endif // SERVER_H