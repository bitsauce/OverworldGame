#include "RedCurrantBush.h"
#include "constants.h"

RedCurrantBush::RedCurrantBush(const int x, const int y) :
	BlockEntity(4, 2),
	m_growTimer(0.0f)
{
	m_sprite = new xd::Sprite(xd::ResourceManager::get<xd::Texture2D>(":/sprites/plants/berry_bush.png"), Rect(), Vector2(), 0.0f, xd::TextureRegion(0.0f, 0.5f, 1.0f, 1.0f));
	m_sprite->setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
	m_sprite->setSize(4 * BLOCK_PXF, 2 * BLOCK_PXF);
}

void RedCurrantBush::draw(SpriteBatch *spriteBatch)
{
	spriteBatch->drawSprite(*m_sprite);
}