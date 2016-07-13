#include "Pickaxe.h"
#include "Constants.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Entities/Player.h"
#include "World/World.h"
#include "Entities/ItemDrop.h"
#include "Game/Game.h"

Pickaxe::Pickaxe(OverworldGame *game, const ItemDataDesc *desc) :
	ItemData(desc),
	m_game(game),
	m_cracksSprite(game->getResourceManager()->get<Texture2D>("Sprites/Items/Tools/Pickaxes/Mining_Cracks")),
	m_sprite(game->getResourceManager()->get<Texture2D>("Sprites/Items/Tools/Pickaxes/IronPickaxe")),
	m_cracksAnimation(1, 4),
	m_drawCracks(false),
	m_prevBlockPosition(0, 0),
	m_mineCounter(0.0f),
	m_mineTime(0.5f)
{
	m_cracksSprite.setRegion(TextureRegion(0.0f, 0.0f, 0.25f, 1.0f), true);
	m_sprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
}

void Pickaxe::equip(Pawn *player)
{
	player->getHumanoid().setAppearanceTexture(Humanoid::ARM_RIGHT, m_sprite.getTexture());
}

void Pickaxe::unequip(Pawn *player)
{
	player->getHumanoid().setAppearanceTexture(Humanoid::ARM_RIGHT, 0);
}

void Pickaxe::update(Pawn *pawn, const float delta)
{
	// Get block input position
	Vector2I position = m_game->getWorld()->getCamera()->getInputPosition();
	position.x = (int) floor(position.x / BLOCK_PXF);
	position.y = (int) floor(position.y / BLOCK_PXF);

	if(pawn->getController()->getInputState(Controller::INPUT_USE_ITEM) && // Do we have user input and...
		m_game->getWorld()->getTerrain()->isBlockAt(position.x, position.y, WORLD_LAYER_MIDDLE)) // ... is there a block at this position?
	{
		// Reset timer if block position have changed
		if(position != m_prevBlockPosition)
		{
			m_mineCounter = m_mineTime;
		}

		// Store previous position
		m_prevBlockPosition = position;

		// Count down and remove block when counter is 0
		m_mineCounter -= delta;
		if(m_mineCounter <= 0.0f)
		{
			m_game->getWorld()->getTerrain()->removeBlockAt(position.x, position.y, WORLD_LAYER_MIDDLE);
		}
		else
		{
			// Update cracks sprite
			m_cracksSprite.setPosition(position * BLOCK_PXF);
			m_cracksSprite.setRegion(m_cracksAnimation.getKeyFrame((int) (4 * (1.0f - m_mineCounter / m_mineTime))));
			m_drawCracks = true;
		}

		// Set mining animation
		m_game->getWorld()->getLocalPlayer()->getHumanoid().setPostAnimation(Humanoid::ANIM_MINE);
	}
	else
	{
		m_mineCounter = m_mineTime; // Reset the counter
		m_game->getWorld()->getLocalPlayer()->getHumanoid().setPostAnimation(Humanoid::ANIM_NULL); // Reset animations
		m_drawCracks = false; // Don't draw cracks
	}
}

void Pickaxe::draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha)
{
	// Is there a block at this position?
	if(m_drawCracks)
	{
		spriteBatch->drawSprite(m_cracksSprite);
	}
}