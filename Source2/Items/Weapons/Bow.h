#ifndef BOW_H
#define BOW_H

#include "Config.h"
#include "Items/ItemData.h"
//>REMOVE ME<
class Bow : public ItemData
{
public:
	Bow(Game *game);
	void use(Player *player, const float delta);
	void draw(Player *player, SpriteBatch *spriteBatch, const float alpha);

private:
	Game *m_game;
	Sprite m_bowSprite;
};

#endif // BOW_H