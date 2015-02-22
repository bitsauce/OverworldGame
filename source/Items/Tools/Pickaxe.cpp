#include "Game.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Game/ItemDrop.h"

Pickaxe::Pickaxe() :
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

void Pickaxe::use(Player *player)
{
	// Get block input position
	Vector2i position = player->getCamera()->getInputPosition();
	position.x = floor(position.x/BLOCK_PXF);
	position.y = floor(position.y/BLOCK_PXF);

	// Is there a block at this position?
	Terrain *terrain = player->getTerrain();
	if(terrain->getBlockAt(position.x, position.y, TERRAIN_LAYER_MIDDLE) > BLOCK_ENTITY)
	{
		// Reset timer if block position have changed
		if(position != m_prevBlockPosition)
		{
			m_mineCounter = m_mineTime;
		}

		// Store previous position
		m_prevBlockPosition = position;

		// Count down and remove block when counter is 0
		m_mineCounter -= Graphics::getTimeStep();
		if(m_mineCounter <= 0.0f)
		{
			ItemID itemID = (ItemID)terrain->getBlockAt(position.x, position.y, TERRAIN_LAYER_MIDDLE);
			terrain->removeBlockAt(position.x, position.y, TERRAIN_LAYER_MIDDLE);
			new ItemDrop(Vector2(position.x * BLOCK_PXF, position.y * BLOCK_PXF), itemID);
		}
		else
		{
			m_cracksSprite.setPosition(position * BLOCK_PXF);
			m_cracksSprite.setRegion(m_cracksAnimation.getKeyFrame(4 * (1.0f - m_mineCounter/m_mineTime)));
		}
	}
}

void Pickaxe::draw(Player *player, SpriteBatch *spriteBatch)
{
	// Is there a block at this position?
	if(player->getTerrain()->isBlockAt(m_prevBlockPosition.x, m_prevBlockPosition.y, TERRAIN_LAYER_MIDDLE))
	{
		spriteBatch->drawSprite(m_cracksSprite);
	}
	player->drawSpriteInHand(m_sprite, Vector2(16.f, 32.f), spriteBatch);
}