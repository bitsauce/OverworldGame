#include "ThingItem.h"
#include "Things/ThingData.h"
#include "World/Terrain/Terrain.h"
#include "Entities/Player.h"
#include "World/Camera.h"
#include "Game/Game.h"

ThingItem::ThingItem(Game *game, const ThingID id) :
	m_game(game),
	m_blockEntityID(id)
{
}

void ThingItem::use(Player *player, const float delta)
{
	// Get block input position
	Vector2i blockPos = m_game->getWorld()->getCamera()->getInputPosition();
	blockPos.x = (int)floor(blockPos.x/BLOCK_PXF);
	blockPos.y = (int)floor(blockPos.y/BLOCK_PXF);

	// Set block if not occupied
	if(m_game->getWorld()->getTerrain()->setThingAt(blockPos.x, blockPos.y, m_blockEntityID))
	{
		// Remove item from player inventory
		//player->getItemStorage()->removeItem(getID(), 1);
		player->getCurrentItem().dec();
	}
}

void ThingItem::draw(Player *player, SpriteBatch *spriteBatch, const float alpha)
{
	// Get block input position
	Vector2i blockPos = m_game->getWorld()->getCamera()->getInputPosition();
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