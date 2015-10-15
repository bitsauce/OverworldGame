#pragma once

#include "DynamicEntity.h"
#include "Game/Game.h"
#include "Pawn.h"

class Arrow : public DynamicEntity
{
public:

	Arrow(Pawn *owner, World *world, const Vector2 &pos, const Vector2 &dir, const float speed);
	void draw(SpriteBatch *spriteBatch, const float alpha);
	bool plotTest(int x, int y);
	void update(const float delta);

	static Entity *Factory(Game *game)
	{
		return new Arrow(0, game->getWorld(), Vector2(), Vector2(), 20.0f);
	}

private:
	Pawn *m_owner;
	Sprite m_sprite;
	float m_angle;
	float m_prevAngle;
	bool m_hasHit;
	float m_deleteTime;
};