#pragma once

#include "Config.h"
#include "Networking/NetworkObject.h"
#include "DynamicEntity.h"

class Pawn;

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

	void setPawn(Pawn *pawn)
	{
		m_pawn = pawn;
	}

	virtual void update() { }

protected:
	Pawn *m_pawn;
	bool m_inputState[INPUT_COUNT];
	void setInputState(const bool state, int type);
};