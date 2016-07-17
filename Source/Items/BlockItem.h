#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H

#include "Config.h"
#include "Items/ItemData.h"

class Camera;
class Terrain;
class BlockData;

class BlockItem : public ItemData
{
public:
	BlockItem(OverworldGame *game, const ItemDataDesc *desc);

	void equip(Pawn *player);
	void unequip(Pawn *player);
	void use(Pawn *pawn, const float delta);
	void draw(Pawn *pawn, SpriteBatch *spriteBatch, const float alpha);

private:
	OverworldGame *const m_game;
	WorldLayer m_layer;
	BlockData *m_blockData;
};

#endif // BLOCK_ITEM_H