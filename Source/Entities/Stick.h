#pragma once

#include "Entity.h"
#include "Game/Game.h"
#include "Pawn.h"

/*class Stick : public Entity
{
public:
	Stick(World *world, const Json::Value &attributes);
	void onDraw(DrawEvent *e);
	void onTick(TickEvent *e);

	static Stick *Factory(World *world, const Json::Value &attributes)
	{
		return new Stick(world, attributes);
	}

private:
	Sprite m_sprite;
	Vector2F m_prevVelocity;
	uint m_hitState;
	float m_deleteTime;
	float m_moveToAlpha;
};*/