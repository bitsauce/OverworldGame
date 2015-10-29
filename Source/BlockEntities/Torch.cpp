#include "Torch.h"

Torch::Torch(World * world, int x, int y, const BlockEntityData *data) :
	BlockEntity(world, x, y, data),
	m_sprite(ResourceManager::get<Texture2D>(":/Sprites/BlockEntities/LightSources/Torch.png")),
	m_pointlight(world->getLighting(), Vector2(), 10.0f, Color(255, 190, 90))
{
	m_sprite.setSize(16, 16);
}

void Torch::update(const float delta)
{

}

void Torch::draw(SpriteBatch *spriteBatch, const float alpha)
{
	// TODO: Fix lighting bug where moving a lightsource should detach the lighting
	// on all the chunks affected by this light.
	m_pointlight.setPosition(getPosition());
	m_sprite.setPosition(getPosition() * BLOCK_PXF);
	spriteBatch->drawSprite(m_sprite);
}