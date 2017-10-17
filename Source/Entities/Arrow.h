#pragma once

#include "Entity.h"
#include "Game/Game.h"
#include "Pawn.h"

class Arrow : public Entity
{
	DECLARE_ENTITY(Arrow)
public:
	void onDraw(DrawEvent *e);
	bool plotTest(int x, int y);
	void onTick(TickEvent *e);

	void packData(RakNet::BitStream *bitStream);
	bool unpackData(RakNet::BitStream *bitStream, const bool force);

private:
	Sprite m_sprite;
	Vector2F m_prevVelocity;
	uint m_hitState;
	float m_deleteTime;
	float m_moveToAlpha;
};