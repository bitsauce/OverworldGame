#ifndef BLOCK_ENTITY_ITEM_H
#define BLOCK_ENTITY_ITEM_H

#include "Constants.h"
#include "Items/ItemData.h"

class BlockEntityItem : public ItemData
{
public:
	BlockEntityItem(OverworldGame *game, const ItemDataDesc *desc);

	virtual void equip(Pawn *player);
	virtual void unequip(Pawn *player);
	virtual void use(Pawn *player, const float delta);
	virtual void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

private:
	OverworldGame * m_game;
	
	BlockEntityID m_blockEntityID;
};

#endif // BLOCK_ENTITY_ITEM_H