#ifndef PICKAXE_H
#define PICKAXE_H

#include "Config.h"
#include "Items/ItemData.h"

class Pickaxe : public ItemData
{
public:
	Pickaxe(OverworldGame * game);
	void equip(Pawn *player);
	void unequip(Pawn *player);
	void update(Pawn *player, const float delta);
	void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

private:
	OverworldGame * m_game;

	Sprite m_sprite;

	Sprite m_cracksSprite;
	SpriteAnimation m_cracksAnimation;
	bool m_drawCracks;

	Vector2i m_prevBlockPosition;
	float m_mineCounter;
	float m_mineTime;
};

#endif // PICKAXE_H