#ifndef AXE_H
#define AXE_H

#include "Items/ItemData.h"

class Axe : public ItemData
{
public:
	Axe(Game *game);

	void equip(Pawn *player);
	void unequip(Pawn *player);
	void update(Pawn *player, const float delta);
	void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

	void makeTreeFall(const int x, const int y);
	void makeLeavesFall(const int x, const int y);

private:
	Game *m_game;

	Sprite m_sprite;

	Sprite m_cracksSprite;
	SpriteAnimation m_cracksAnimation;
	bool m_drawCracks;

	Vector2i m_prevBlockPosition;
	float m_mineCounter;
	float m_mineTime;
};

#endif // AXE_H