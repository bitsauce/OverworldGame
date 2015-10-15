#include "Pickaxe.h"
#include "Constants.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Entities/Dynamic/Player.h"
#include "World/Camera.h"
#include "Entities/Dynamic/ItemDrop.h"
#include "World/Terrain/Terrain.h"
#include "Game/Game.h"

Pickaxe::Pickaxe(Game *game) :
	m_game(game),
	m_cracksSprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Tools/Pickaxes/MiningCracks.png")),
	m_cracksAnimation(1, 4),
	m_drawCracks(false),
	m_sprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Tools/Pickaxes/IronPickaxe.png")),
	m_prevBlockPosition(0, 0),
	m_mineCounter(0.0f),
	m_mineTime(0.1f)
{
	m_cracksSprite.setRegion(TextureRegion(0.0f, 0.0f, 0.25f, 1.0f), true);
	m_sprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
}

void Pickaxe::equip(Pawn *player)
{
	player->getHumanoid().setAttachmentTexture(Humanoid::ARM_RIGHT, 1, m_sprite.getTexture());
}

void Pickaxe::unequip(Pawn *player)
{
	player->getHumanoid().setAttachmentTexture(Humanoid::ARM_RIGHT, 1, 0);
}

void Pickaxe::update(Pawn *pawn, const float delta)
{
	// Get block input position
	Vector2i position = m_game->getWorld()->getCamera()->getInputPosition();
	position.x = (int) floor(position.x / BLOCK_PXF);
	position.y = (int) floor(position.y / BLOCK_PXF);

	if(pawn->getController()->getInputState(Controller::INPUT_USE_ITEM) && // Do we have user input and...
		m_game->getWorld()->getTerrain()->getBlockAt(position.x, position.y, TERRAIN_LAYER_MIDDLE) > BLOCK_ENTITY) // ... is there a block at this position?
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
			m_game->getWorld()->getTerrain()->removeBlockAt(position.x, position.y, TERRAIN_LAYER_MIDDLE);
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