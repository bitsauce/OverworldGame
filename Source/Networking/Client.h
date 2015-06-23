#ifndef CLIENT_H
#define CLIENT_H

#include "Config.h"
#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}
//>REMOVE ME<
class Game;

class Client : public Connection
{
public:
	Client(Game *game, const string &ip, const ushort port);

	void update();
	void sendPacket(RakNet::BitStream *bitStream);

private:
	Game *m_game;
};

#endif // CLIENT_H