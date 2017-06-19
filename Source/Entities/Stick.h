#pragma once

#include "Entity.h"
#include "Game/Game.h"
#include "Pawn.h"

class Stick : public Entity
{
public:
	Stick(Pawn *owner, World *world, const Vector2F &pos, const Vector2F &dir, const float speed);
	void onDraw(DrawEvent *e);
	void onTick(TickEvent *e);

	static Stick *Factory(World *world)
	{
		return new Stick(0, world, Vector2F(), Vector2F(), 20.0f);
	}

private:
	Pawn *m_owner;
	Sprite m_sprite;
	Vector2F m_prevVelocity;
	uint m_hitState;
	float m_deleteTime;
	float m_moveToAlpha;
};