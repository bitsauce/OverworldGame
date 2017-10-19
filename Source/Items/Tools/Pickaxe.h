#pragma once

#include "Config.h"
#include "Items/ItemData.h"

class Pickaxe : public Item
{
public:
	Pickaxe(World *world, Pawn *pawn);
	void equip();
	void unequip();
	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	static Item *Factory(World *world, Pawn *pawn) { return new Pickaxe(world, pawn); }

private:
	Sprite m_sprite;
	Sprite m_cracksSprite;
	SpriteAnimation m_cracksAnimation;
	bool m_drawCracks;
	Vector2I m_prevBlockPosition;
	float m_mineCounter;
	float m_mineTime;
};