#ifndef CLIENT_H
#define CLIENT_H

#include "Config.h"
#include "Connection.h"

namespace RakNet {
	class RakPeerInterface;
	class BitStream;
}

class Player;
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
	float getJoinProgress() const { return (float)m_joinProgress / m_joinNumSteps; }
	int getPing() const { return m_rakPeer->GetAveragePing(m_serverGuid); }

private:
	bool pushingPastCapacity();

	Overworld *m_game;
	string m_playerName;
	bool m_joinFinalized;
	int m_joinProgress;
	int m_joinNumSteps;
	RakNet::RakNetGUID m_serverGuid;
	RakNet::SystemAddress m_serverAddress;
	uint m_increasePacketRateCounter = 0;
	uint m_ticksBeforeIncreasePacketRate = 0;

	map<RakNet::RakNetGUID, Player*> m_players;
};

#endif // CLIENT_H