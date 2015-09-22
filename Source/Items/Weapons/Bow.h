#ifndef BOW_H
#define BOW_H

#include "Config.h"
#include "Items/ItemData.h"

class Bow : public ItemData
{
public:
	Bow(Game *game);
	void use(Pawn *player, const float delta);
	void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

private:
	Game *m_game;
	Sprite m_bowSprite;
};

#endif // BOW_H