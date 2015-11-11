#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H

#include "Config.h"
#include "Items/ItemData.h"

class Camera;
class Terrain;
enum BlockID;
enum TerrainLayer;

class BlockItem : public ItemData
{
public:
	BlockItem(OverworldGame * game, const WorldLayer layer, const BlockID blockID);

	void use(Pawn *pawn, const float delta);
	void draw(Pawn *pawn, SpriteBatch *spriteBatch, const float alpha);

private:
	Camera *m_camera;
	Terrain *m_terrain;
	const WorldLayer m_layer;
	const BlockID m_blockID;
};

#endif // BLOCK_ITEM_H