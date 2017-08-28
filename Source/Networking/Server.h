#ifndef SERVER_H
#define SERVER_H

#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class Overworld;
class Player;

class Server : public Connection
{
public:
	enum ErrorCode
	{
		SUCCESS,
		COULD_NOT_SETUP_SOCKET
	};

	Server(Overworld *game);

	ErrorCode host(const string &worldName, const ushort port);

	void onTick(TickEvent *e);
	//void onDraw() { NO! }

	void sendPacket(RakNet::BitStream *bitStream);
	void save();

private:
	void savePlayer(Player *player);

	Overworld * m_game;
	map<RakNet::RakNetGUID, Player*> m_players;
};

#endif // SERVER_H