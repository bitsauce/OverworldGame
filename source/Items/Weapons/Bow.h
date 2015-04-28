#ifndef BOW_H
#define BOW_H

#include "Config.h"
#include "Items/ItemData.h"

class Bow : public ItemData
{
public:
	Bow(Game *game);
	void use(ItemContainer::Slot *slot, const float delta);
	void draw(Player *player, SpriteBatch *spriteBatch, const float alpha);

private:
	Game *m_game;
	Sprite m_bowSprite;
};

#endif // BOW_H