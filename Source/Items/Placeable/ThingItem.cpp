#include "ThingItem.h"
#include "Things/ThingData.h"
#include "World/Terrain/Terrain.h"
#include "Entities/Player.h"
#include "World/Camera.h"

ThingItem::ThingItem(const ThingID id) :
	m_blockEntityID(id)
{
}

void ThingItem::use(Player *player)
{
	// Get block input position
	Vector2i blockPos = player->getCamera()->getInputPosition();
	blockPos.x = (int)floor(blockPos.x/BLOCK_PXF);
	blockPos.y = (int)floor(blockPos.y/BLOCK_PXF);

	// Set block if not occupied
	Terrain *terrain = player->getTerrain();
	if(terrain->setThingAt(blockPos.x, blockPos.y, m_blockEntityID))
	{
		// Remove item from player inventory
		player->getItemContainer().removeItem(getID(), 1);
	}
}

void ThingItem::draw(Player *player, SpriteBatch *spriteBatch)
{
	// Get block input position
	Vector2i blockPos = player->getCamera()->getInputPosition();
	blockPos.x = (int)floor(blockPos.x/BLOCK_PXF);
	blockPos.y = (int)floor(blockPos.y/BLOCK_PXF);

	// Draw preview sprite
	ThingData &data = ThingData::get(m_blockEntityID);

	Sprite sprite = data.getSprite();
	sprite.setColor(data.canPlace(blockPos.x, blockPos.y) ? Color(240, 240, 240, 215) : Color(255, 90, 90, 215));
	sprite.setPosition(blockPos.x * BLOCK_PXF, blockPos.y * BLOCK_PXF);
	spriteBatch->drawSprite(sprite);
	
	sprite = data.getSprite();
	sprite.setSize(16.0f, 16.0f);
	player->getHumanoid().drawRightHandSprite(sprite, Vector2(8.0f, 16.0f), spriteBatch);

	if(m_blockEntityID == BLOCK_ENTITY_TORCH)
	{
		//player->setItemAnimation(player->getSkeleton()->findAnimation("hold-torch"));
	}
}