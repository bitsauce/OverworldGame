#pragma once

#include "Config.h"
#include "Items/ItemData.h"

class World;

class Pickaxe : public ItemData
{
public:
	Pickaxe(const ItemDataDesc *desc);
	void equip(Pawn *player);
	void unequip(Pawn *player);
	void update(World *world, Pawn *player, const float delta);
	void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

private:
	Sprite m_sprite;

	Sprite m_cracksSprite;
	SpriteAnimation m_cracksAnimation;
	bool m_drawCracks;

	Vector2I m_prevBlockPosition;
	float m_mineCounter;
	float m_mineTime;
};