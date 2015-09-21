#pragma once

#include "Config.h"
#include "Networking/NetworkObject.h"
#include "Entities/Physics/DynamicEntity.h"

class Player;

class Controller : public NetworkObject
{
public:
	Controller();

	void pack(RakNet::BitStream *bitStream, const Connection *conn);
	void unpack(RakNet::BitStream *bitStream, const Connection *conn);

	enum
	{
		INPUT_MOVE_LEFT,
		INPUT_MOVE_RIGHT,
		INPUT_JUMP,
		INPUT_RUN,
		INPUT_USE_ITEM,
		INPUT_COUNT
	};

	bool getInputState(int type) const
	{
		return m_inputState[type];
	}

	void setPlayer(Player *player)
	{
		m_player = player;
	}

	virtual void update() { }

private:
	bool m_inputState[INPUT_COUNT];
	bool m_clientInputState[INPUT_COUNT];

protected:
	RakNet::RakNetGUID m_guid;
	Player *m_player;
	void setClientInputState(int action, int type);
	void setClientUseItemState(int action);
};