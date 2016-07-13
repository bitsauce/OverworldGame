#ifndef BLOCK_ENTITY_ITEM_H
#define BLOCK_ENTITY_ITEM_H

#include "Items/ItemData.h"

enum EntityID;

class BlockEntityItem : public ItemData
{
public:
	BlockEntityItem(OverworldGame *game, const ItemDataDesc *desc);

	void equip(Pawn *player);
	void unequip(Pawn *player);
	void use(Pawn *player, const float delta);
	void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

private:
	OverworldGame * m_game;
	
	BlockEntityID m_blockEntityID;
};

#endif // BLOCK_ENTITY_ITEM_H