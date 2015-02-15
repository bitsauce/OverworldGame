#ifndef BLOCK_ENTITY_ITEM_H
#define BLOCK_ENTITY_ITEM_H

#include "Items/ItemData.h"

enum BlockEntityID;

class BlockEntityItem : public ItemData
{
public:
	BlockEntityItem(const BlockEntityID id);

	void use(Player *player);
	void draw(Player *player, SpriteBatch *spriteBatch);

private:
	const BlockEntityID m_blockEntityID;
};

#endif // BLOCK_ENTITY_ITEM_H