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

	void use(Pawn *pawn, const float delta);
	void draw(Pawn *pawn, SpriteBatch *spriteBatch, const float alpha);

private:
	Camera *m_camera;
	Terrain *m_terrain;
	WorldLayer m_layer;
	BlockData *m_blockData;
};

#endif // BLOCK_ITEM_H