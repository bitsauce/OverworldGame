#ifndef BLOCK_ENTITY_ITEM_H
#define BLOCK_ENTITY_ITEM_H

#include "Constants.h"
#include "Items/ItemData.h"
#include "BlockEntities/BlockEntityData.h"

class BlockEntityItem : public ItemData
{
public:
	BlockEntityItem(Overworld *game, const ItemDataDesc *desc);

	virtual void equip(Pawn *player);
	virtual void unequip(Pawn *player);
	virtual void use(Pawn *player, const float delta);
	virtual void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

private:
	Overworld *m_game;
	BlockEntityData *m_blockEntityData;
};

#endif // BLOCK_ENTITY_ITEM_H