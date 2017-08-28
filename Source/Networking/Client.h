#ifndef CLIENT_H
#define CLIENT_H

#include "Config.h"
#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class Overworld;

class Client : public Connection
{
public:
	enum ErrorCode
	{
		SUCCESS,
		COULD_NOT_SETUP_SOCKET,
		COULD_NOT_CONNECT
	};

	Client(Overworld *game);

	ErrorCode join(const string &playerName, const string &ip, const ushort port);

	void onTick(TickEvent *e);
	void sendPacket(RakNet::BitStream *bitStream);

	bool isJoinFinalized() const { return m_joinFinalized; }

private:
	Overworld *m_game;
	string m_playerName;
	bool m_joinFinalized;
};

#endif // CLIENT_H