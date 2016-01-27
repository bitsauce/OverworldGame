#pragma once

#include "DynamicEntity.h"
#include "Game/Game.h"
#include "Pawn.h"

class Arrow : public DynamicEntity
{
public:

	Arrow(Pawn *owner, World *world, const Vector2F &pos, const Vector2F &dir, const float speed);
	void onDraw(DrawEvent *e);
	bool plotTest(int x, int y);
	void onTick(TickEvent *e);

	static DynamicEntity *Factory(World * world)
	{
		return new Arrow(0, world, Vector2F(), Vector2F(), 20.0f);
	}

private:
	Pawn *m_owner;
	Sprite m_sprite;
	float m_angle;
	float m_prevAngle;
	bool m_hasHit;
	float m_deleteTime;
};