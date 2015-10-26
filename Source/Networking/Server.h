#ifndef SERVER_H
#define SERVER_H

#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class OverworldGame;
class Pawn;

class Server : public Connection
{
public:
	Server(OverworldGame * game, const ushort port);

	void update();
	void sendPacket(RakNet::BitStream *bitStream);

	void save();

	RakNet::RakPeerInterface *getRakPeer() const { return m_rakPeer; }

private:
	void savePlayer(string playerName);

	OverworldGame * m_game;
	map<string, Pawn*> m_players;
};

#endif // SERVER_H