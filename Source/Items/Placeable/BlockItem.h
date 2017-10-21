#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H

#include "Config.h"
#include "Items/ItemData.h"

class Camera;
class Terrain;
class BlockData;

class BlockItem : public Item
{
public:
	BlockItem(World *world, Pawn *pawn, const Json::Value &attributes);

	void equip();
	void unequip();
	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);


	static Item *Factory(World *world, Pawn *pawn, const Json::Value &attributes) { return new BlockItem(world, pawn, attributes); }

private:
	WorldLayer m_layer;
	BlockData *m_blockData;
};

#endif // BLOCK_ITEM_H