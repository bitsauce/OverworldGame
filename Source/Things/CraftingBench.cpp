#include "CraftingBench.h"
#include "ThingData.h"
#include "Constants.h"
#include "Entities/Player.h"

CraftingBench::CraftingBench(const int x, const int y) :
	Thing(x, y),
	m_sprite(ThingData::get(BLOCK_ENTITY_CRAFTING_BENCH).getSprite())
{
	m_sprite.setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
}

void CraftingBench::activate(Player *player)
{
	player->setCrafting(!player->isCrafting());
}

void CraftingBench::draw(SpriteBatch *spriteBatch, const float alpha)
{
	spriteBatch->drawSprite(m_sprite);
}