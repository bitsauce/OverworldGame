#include "redcurrantbush.h"
#include "constants.h"

RedcurrantBush::RedcurrantBush() :
	BlockEntity(4, 2),
	m_growTimer(0.0f)
{
	m_sprite = new xd::Sprite(xd::ResourceManager::get<xd::Texture2D>(":/sprites/plants/berry_bush.png"), Rect(), Vector2(), 0.0f, xd::TextureRegion(0.0f, 0.5f, 1.0f, 1.0f));
	m_sprite->setSize(4 * BLOCK_PXF, 2 * BLOCK_PXF);
}

void RedcurrantBush::moveEvent()
{
	m_sprite->setColor(isPlaceable() ? xd::Color(255, 255, 255, 127) : xd::Color(255, 127, 127, 127));
	m_sprite->setPosition(getX() * BLOCK_PXF, getY() * BLOCK_PXF);
}

void RedcurrantBush::placeEvent()
{
	m_sprite->setColor(xd::Color(255, 255, 255, 255));
}

void RedcurrantBush::draw(xd::SpriteBatch *spriteBatch)
{
	spriteBatch->drawSprite(*m_sprite);
}