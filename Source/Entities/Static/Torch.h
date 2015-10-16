#pragma once

#include "StaticEntity.h"
#include "Game/Game.h"

class Torch : public StaticEntity
{
public:
	Torch(World * world, int x, int y);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	static StaticEntity* Factory(World * world, const int x, const int y)
	{
		return new Torch(world, x, y);
	}

private:
	Sprite m_sprite;
	Pointlight m_pointlight;
};