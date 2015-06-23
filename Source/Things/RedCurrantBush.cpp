#include "RedCurrantBush.h"
#include "ThingData.h"
#include "Constants.h"

RedCurrantBush::RedCurrantBush(const int x, const int y) :
	Thing(x, y),
	m_sprite(ThingData::get(BLOCK_ENTITY_RED_CURRANT_BUSH).getSprite()),
	m_growTimer(0.0f)
{
	m_sprite.setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
}

void RedCurrantBush::draw(SpriteBatch *spriteBatch)
{
	spriteBatch->drawSprite(m_sprite);
}