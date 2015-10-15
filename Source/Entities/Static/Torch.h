#pragma once

#include "StaticEntity.h"
#include "Game/Game.h"

class Torch : public StaticEntity
{
public:
	Torch(World * world);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	static Entity* Factory(Game* game)
	{
		return new Torch(game->getWorld());
	}

private:
	Sprite m_sprite;
	Pointlight m_pointlight;
};