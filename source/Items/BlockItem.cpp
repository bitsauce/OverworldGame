#include "BlockItem.h"
#include "Entities/Player.h"
#include "World/Camera.h"
#include "Terrain/Terrain.h"
#include "Blocks/BlockData.h"

BlockItem::BlockItem(const TerrainLayer layer, const BlockID blockID) :
	m_layer(layer),
	m_blockID(blockID)
{
}

void BlockItem::use(Player *player)
{
	// Get block input position
	Vector2i blockPos = player->getCamera()->getInputPosition();
	blockPos.x = floor(blockPos.x/BLOCK_PXF);
	blockPos.y = floor(blockPos.y/BLOCK_PXF);

	// Set block if not occupied
	Terrain *terrain = player->getTerrain();
	if(!terrain->isBlockAt(blockPos.x, blockPos.y, m_layer))
	{
		terrain->setBlockAt(blockPos.x, blockPos.y, m_blockID, m_layer);

		// Remove item from player inventory
		player->getItemContainer().removeItem(getID(), 1);
	}
}

void BlockItem::draw(Player *player, SpriteBatch *spriteBatch)
{
	player->getHumanoid().drawRightHandSprite(Sprite(BlockData::getBlockAtlas()->getTexture(), Rect(0,0,16,16), Vector2(), 0.0, BlockData::getBlockAtlas()->get(m_blockID, 0,0,1,0.6)), Vector2(8.0f, 16.0f), spriteBatch);
}