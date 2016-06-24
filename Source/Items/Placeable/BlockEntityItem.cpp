#include "BlockEntityItem.h"
#include "World/World.h"
#include "Entities/Pawn.h"
#include "Game/Game.h"
#include "BlockEntities/BlockEntityData.h"

BlockEntityItem::BlockEntityItem(OverworldGame *game, const BlockEntityID id) :
	m_game(game),
	m_blockEntityID(id)
{
}

void BlockEntityItem::equip(Pawn *pawn)
{
	if(m_blockEntityID == BLOCK_ENTITY_TORCH)
	{
		pawn->m_pointLight->setRadius(25);
		pawn->m_pointLight->setColor(Color(255, 200, 15, 255));
	}

	pawn->getHumanoid().setAttachmentTexture(Humanoid::ARM_RIGHT, 1, getIconTexture());
}

void BlockEntityItem::unequip(Pawn *pawn)
{
	if(m_blockEntityID == BLOCK_ENTITY_TORCH)
	{
		pawn->m_pointLight->setRadius(10);
		pawn->m_pointLight->setColor(Color(127));
	}

	pawn->getHumanoid().setAttachmentTexture(Humanoid::ARM_RIGHT, 1, 0);
}

void BlockEntityItem::use(Pawn *pawn, const float delta)
{
	// Get block input position
	Vector2I blockPos = math::floor(Vector2F(m_game->getWorld()->getCamera()->getInputPosition()) / BLOCK_PXF);

	// Set block if not occupied
	/*if(m_game->getWorld()->getTerrain()->setThingAt(blockPos.x, blockPos.y, m_entityID))
	{
		// Remove item from player inventory
		player->getCurrentItem()->dec();
	}*/

	if(m_game->getWorld()->getTerrain()->createBlockEntityAt(blockPos.x, blockPos.y, m_blockEntityID))
	{
		pawn->getCurrentItem()->dec();
	}
}

void BlockEntityItem::draw(Pawn *pawn, SpriteBatch *spriteBatch, const float alpha)
{
	// Get block input position
	/*Vector2i blockPos = m_game->getWorld()->getCamera()->getInputPosition();
	blockPos.x = (int)floor(blockPos.x/BLOCK_PXF);
	blockPos.y = (int)floor(blockPos.y/BLOCK_PXF);

	// Draw preview sprite
	ThingData &data = ThingData::get(m_entityID);

	Sprite sprite = data.getSprite();
	sprite.setColor(data.canPlace(blockPos.x, blockPos.y) ? Color(240, 240, 240, 215) : Color(255, 90, 90, 215));
	sprite.setPosition(blockPos.x * BLOCK_PXF, blockPos.y * BLOCK_PXF);
	spriteBatch->drawSprite(sprite);
	
	sprite = data.getSprite();
	sprite.setSize(16.0f, 16.0f);
	//player->getHumanoid().drawRightHandSprite(sprite, Vector2(8.0f, 16.0f), spriteBatch);

	if(m_entityID == BLOCK_ENTITY_TORCH)
	{
		//player->setItemAnimation(player->getSkeleton()->findAnimation("hold-torch"));
	}*/
}