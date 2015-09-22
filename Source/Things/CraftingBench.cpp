#include "CraftingBench.h"
#include "ThingData.h"
#include "Constants.h"
#include "Entities/Pawn.h"
#include "Game/Game.h"

CraftingBench::CraftingBench(Game *game, const int x, const int y) :
	Thing(x, y),
	m_game(game),
	m_sprite(ThingData::get(BLOCK_ENTITY_CRAFTING_BENCH).getSprite())
{
	m_sprite.setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
}

void CraftingBench::activate(Pawn *player)
{
	m_game->getGameOverlay()->toggleCrafting();
}

void CraftingBench::draw(SpriteBatch *spriteBatch, const float alpha)
{
	spriteBatch->drawSprite(m_sprite);
}