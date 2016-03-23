#include "BlockItem.h"
#include "Entities/Pawn.h"
#include "World/World.h"
#include "Blocks/BlockData.h"
#include "Game/Game.h"

BlockItem::BlockItem(OverworldGame * game, const WorldLayer layer, const BlockID blockID) :
	m_camera(game->getWorld()->getCamera()),
	m_terrain(game->getWorld()->getTerrain()),
	m_layer(layer),
	m_blockID(blockID)
{
}

void BlockItem::use(Pawn *pawn, const float delta)
{
	// Get block input position
	Vector2I blockPos = m_camera->getInputPosition();
	blockPos.x = (int) floor(blockPos.x/BLOCK_PXF);
	blockPos.y = (int) floor(blockPos.y/BLOCK_PXF);

	// Set block if not occupied
	if(m_terrain->setBlockAt(blockPos.x, blockPos.y, m_blockID, m_layer))
	{
		// Remove item from player inventory
		pawn->getCurrentItem()->dec();
	}
}

void BlockItem::draw(Pawn *pawn, SpriteBatch *spriteBatch, const float alpha)
{
	//pawn->getHumanoid().drawRightHandSprite(Sprite(BlockData::getBlockAtlas()->getTexture(), Rect(0,0,16,16), Vector2(), 0.0f, BlockData::getBlockAtlas()->get(m_blockID, 0,0,1,0.6f)), Vector2(8.0f, 16.0f), spriteBatch);
}