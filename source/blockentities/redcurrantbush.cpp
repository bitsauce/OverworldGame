#include "redcurrantbush.h"
#include "constants.h"

RedcurrantBush::RedcurrantBush() :
	BlockEntity(4, 2),
	m_growTimer(0.0f)
{
	m_sprite = new XSprite(XTextureRegion(xd::ResourceManager::get<XTexture>(":/sprites/plants/berry_bush.png"), 0.0f, 0.5f, 1.0f, 1.0f));
	m_sprite->setSize(4 * BLOCK_PXF, 2 * BLOCK_PXF);
}

void RedcurrantBush::moveEvent()
{
	m_sprite->setColor(isPlaceable() ? Vector4(1.0f, 1.0f, 1.0f, 0.5f) : Vector4(1.0f, 0.5f, 0.5f, 0.5f));
	m_sprite->setPosition(getX() * BLOCK_PXF, getY() * BLOCK_PXF);
}

void RedcurrantBush::placeEvent()
{
	m_sprite->setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}

void RedcurrantBush::draw(XBatch *batch)
{
	m_sprite->draw(batch);
}