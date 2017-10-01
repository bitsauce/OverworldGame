#pragma once

#include "Config.h"
#include "Networking/NetworkObject.h"
#include "Entities/Entity.h"

class Pawn;

class Controller// : public NetworkObject
{
	friend class Server;
	friend class Client;
public:
	Controller();

	enum
	{
		INPUT_MOVE_LEFT,
		INPUT_MOVE_RIGHT,
		INPUT_JUMP,
		INPUT_RUN,
		INPUT_USE_ITEM,
		INPUT_RMB,
		INPUT_COUNT
	};

	bool getInputState(uint type) const
	{
		return (m_inputState & (1 << type)) != 0;
	}

	void setPawn(Pawn *pawn)
	{
		m_pawn = pawn;
	}

	Vector2F getInputPosition() const
	{
		return m_inputPosition;
	}

	virtual void update() { }

protected:
	Pawn *m_pawn;
	uint m_inputState;
	Vector2I m_inputPosition;
	void setInputState(const bool state, int type);
};