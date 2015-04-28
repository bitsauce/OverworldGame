#ifndef BLOCK_ENTITY_ITEM_H
#define BLOCK_ENTITY_ITEM_H

#include "Items/ItemData.h"

enum ThingID;

class ThingItem : public ItemData
{
public:
	ThingItem(Game *game, const ThingID id);

	void use(ItemContainer::Slot *slot, const float delta);
	void draw(Player *player, SpriteBatch *spriteBatch, const float alpha);

private:
	Game *m_game;
	const ThingID m_blockEntityID;
};

#endif // BLOCK_ENTITY_ITEM_H