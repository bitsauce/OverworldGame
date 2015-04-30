#include "Pickaxe.h"
#include "Constants.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Entities/Player.h"
#include "World/Camera.h"
#include "Entities/ItemDrop.h"
#include "World/Terrain/Terrain.h"
#include "Game/Game.h"

Pickaxe::Pickaxe(Game *game) :
	m_game(game),
	m_cracksSprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Tools/Pickaxes/MiningCracks.png")),
	m_cracksAnimation(1, 4),
	m_sprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Tools/Pickaxes/IronPickaxe.png")),
	m_prevBlockPosition(0.0f, 0.0f),
	m_mineCounter(0.0f),
	m_mineTime(0.1f)
{
	m_cracksSprite.setRegion(TextureRegion(0.0f, 0.0f, 0.25f, 1.0f), true);
	m_sprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
}

void Pickaxe::use(ItemContainer::Slot *slot, const float delta)
{
	// Get block input position
	Vector2i position = m_game->getWorld()->getCamera()->getInputPosition();
	position.x = floor(position.x/BLOCK_PXF);
	position.y = floor(position.y/BLOCK_PXF);

	// Is there a block at this position?
	if(m_game->getWorld()->getTerrain()->getBlockAt(position.x, position.y, TERRAIN_LAYER_MIDDLE) > BLOCK_ENTITY)
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
			m_cracksSprite.setPosition(position * BLOCK_PXF);
			m_cracksSprite.setRegion(m_cracksAnimation.getKeyFrame(4 * (1.0f - m_mineCounter/m_mineTime)));
		}

		m_game->getWorld()->getLocalPlayer()->getHumanoid().setPostAnimation(Humanoid::ANIM_MINE);
	}
	else
	{
		m_game->getWorld()->getLocalPlayer()->getHumanoid().setPostAnimation(Humanoid::ANIM_NULL);
	}
}

void Pickaxe::draw(Player *player, SpriteBatch *spriteBatch, const float alpha)
{
	// Is there a block at this position?
	if(m_game->getWorld()->getTerrain()->isBlockAt(m_prevBlockPosition.x, m_prevBlockPosition.y, TERRAIN_LAYER_MIDDLE))
	{
		spriteBatch->drawSprite(m_cracksSprite);
	}
	player->getHumanoid().drawRightHandSprite(m_sprite, Vector2(16.f, 32.f), spriteBatch);
}