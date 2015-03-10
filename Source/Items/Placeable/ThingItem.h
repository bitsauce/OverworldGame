#ifndef BLOCK_ENTITY_ITEM_H
#define BLOCK_ENTITY_ITEM_H

#include "Items/ItemData.h"

enum ThingID;

class ThingItem : public ItemData
{
public:
	ThingItem(const ThingID id);

	void use(Player *player, const float dt);
	void draw(Player *player, SpriteBatch *spriteBatch, const float alpha);

private:
	const ThingID m_blockEntityID;
};

#endif // BLOCK_ENTITY_ITEM_H