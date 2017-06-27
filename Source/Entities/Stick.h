#pragma once

#include "Entity.h"
#include "Game/Game.h"
#include "Pawn.h"

class Stick : public Entity
{
public:
	Stick(const Json::Value &attributes);
	void onDraw(DrawEvent *e);
	void onTick(TickEvent *e);

	static Stick *Factory(const Json::Value &attributes)
	{
		return new Stick(attributes);
	}

private:
	Sprite m_sprite;
	Vector2F m_prevVelocity;
	uint m_hitState;
	float m_deleteTime;
	float m_moveToAlpha;
};