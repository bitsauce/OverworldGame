#ifndef BOW_H
#define BOW_H

#include "Config.h"
#include "Items/ItemData.h"

class Bow : public ItemData
{
public:
	Bow();
	void use(Player *player);
	void draw(Player *player, SpriteBatch *spriteBatch);

private:
	Sprite m_bowSprite;
};

#endif // BOW_H