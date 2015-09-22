#ifndef BLOCK_ENTITY_ITEM_H
#define BLOCK_ENTITY_ITEM_H

#include "Items/ItemData.h"

enum ThingID;

class ThingItem : public ItemData
{
public:
	ThingItem(Game *game, const ThingID id);

	void use(Pawn *player, const float delta);
	void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

private:
	Game *m_game;
	const ThingID m_blockEntityID;
};

#endif // BLOCK_ENTITY_ITEM_H