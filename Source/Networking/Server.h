#ifndef SERVER_H
#define SERVER_H

#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class OverworldGame;
class Player;

class Server : public Connection
{
public:
	Server(OverworldGame * game, const ushort port);

	void onTick(TickEvent *e);

	void sendPacket(RakNet::BitStream *bitStream);
	void save();

private:
	void savePlayer(Player *player);

	OverworldGame * m_game;
	map<RakNet::RakNetGUID, Player*> m_players;
};

#endif // SERVER_H