#include "RedCurrantBush.h"
#include "constants.h"

RedCurrantBush::RedCurrantBush(const int x, const int y) :
	BlockEntity(4, 2),
	m_sprite(BlockEntityData::get(BLOCK_ENTITY_RED_CURRANT_BUSH).getSprite()),
	m_growTimer(0.0f)
{
	m_sprite.setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
}

void RedCurrantBush::draw(SpriteBatch *spriteBatch)
{
	spriteBatch->drawSprite(m_sprite);
}