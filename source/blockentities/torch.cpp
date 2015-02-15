#include "Game.h"

Torch::Torch(const int x, const int y) :
	BlockEntity(x, y),
	m_sprite(BlockEntityData::get(BLOCK_ENTITY_TORCH).getSprite()),
	m_spotlight(Vector2(x, y), 10, Color(255, 190, 90))
{
	m_sprite.setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
}

void Torch::draw(SpriteBatch *spriteBatch)
{
	spriteBatch->drawSprite(m_sprite);
}