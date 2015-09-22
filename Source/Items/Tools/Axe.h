#ifndef AXE_H
#define AXE_H

#include "Items/ItemData.h"

class Axe : public ItemData
{
public:
	Axe(Game *game);

	void use(Pawn *player, const float delta);
	void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

	void makeTreeFall(const int x, const int y);

private:
	Game *m_game;
	Sprite m_cracksSprite;
	SpriteAnimation m_cracksAnimation;
	Sprite m_sprite;
	Vector2i m_prevBlockPosition;
	float m_mineCounter;
	float m_mineTime;
};

#endif // AXE_H