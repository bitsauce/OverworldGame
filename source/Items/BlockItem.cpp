#include "BlockItem.h"
#include "Entities/Player.h"
#include "World/Camera.h"
#include "World/Terrain/Terrain.h"
#include "Blocks/BlockData.h"
#include "Game/Game.h"

BlockItem::BlockItem(Game *game, const TerrainLayer layer, const BlockID blockID) :
	m_camera(game->getWorld()->getCamera()),
	m_terrain(game->getWorld()->getTerrain()),
	m_layer(layer),
	m_blockID(blockID)
{
}

void BlockItem::use(Player *player, const float delta)
{
	// Get block input position
	Vector2i blockPos = m_camera->getInputPosition();
	blockPos.x = (int) floor(blockPos.x/BLOCK_PXF);
	blockPos.y = (int) floor(blockPos.y/BLOCK_PXF);

	// Set block if not occupied
	if(!m_terrain->isBlockAt(blockPos.x, blockPos.y, m_layer))
	{
		m_terrain->setBlockAt(blockPos.x, blockPos.y, m_blockID, m_layer);

		// Remove item from player inventory
		player->getCurrentItem().dec();
		//player->getItemStorage()->removeItem(getID(), 1);
	}
}

void BlockItem::draw(Player *player, SpriteBatch *spriteBatch, const float alpha)
{
	player->getHumanoid().drawRightHandSprite(Sprite(BlockData::getBlockAtlas()->getTexture(), Rect(0,0,16,16), Vector2(), 0.0f, BlockData::getBlockAtlas()->get(m_blockID, 0,0,1,0.6f)), Vector2(8.0f, 16.0f), spriteBatch);
}