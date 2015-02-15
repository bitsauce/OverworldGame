#include "Game.h"

BlockEntityItem::BlockEntityItem(const BlockEntityID id) :
	m_blockEntityID(id)
{
}

void BlockEntityItem::use(Player *player)
{
	// Get block input position
	Vector2i blockPos = player->getCamera()->getInputPosition();
	blockPos.x = floor(blockPos.x/BLOCK_PXF);
	blockPos.y = floor(blockPos.y/BLOCK_PXF);

	// Set block if not occupied
	Terrain *terrain = player->getTerrain();
	if(terrain->setBlockEntityAt(blockPos.x, blockPos.y, m_blockEntityID))
	{
		// Remove item from player inventory
		player->getItemContainer().removeItem(getID(), 1);
	}
}

void BlockEntityItem::draw(Player *player, SpriteBatch *spriteBatch)
{
	// Get block input position
	Vector2i blockPos = player->getCamera()->getInputPosition();
	blockPos.x = floor(blockPos.x/BLOCK_PXF);
	blockPos.y = floor(blockPos.y/BLOCK_PXF);

	// Draw preview sprite
	BlockEntityData &data = BlockEntityData::get(m_blockEntityID);
	Sprite sprite = data.getSprite();
	sprite.setColor(data.canPlace(blockPos.x, blockPos.y) ? Color(240, 240, 240, 215) : Color(255, 90, 90, 215));
	sprite.setPosition(blockPos.x * BLOCK_PXF, blockPos.y * BLOCK_PXF);
	spriteBatch->drawSprite(sprite);
}