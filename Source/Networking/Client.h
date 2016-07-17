#ifndef CLIENT_H
#define CLIENT_H

#include "Config.h"
#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class OverworldGame;

class Client : public Connection
{
public:
	Client(OverworldGame * game, const string &ip, const ushort port);

	void onTick(TickEvent *e);
	void sendPacket(RakNet::BitStream *bitStream);

private:
	OverworldGame * m_game;
};

#endif // CLIENT_H