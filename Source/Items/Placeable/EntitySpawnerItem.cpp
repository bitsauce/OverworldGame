#include "EntitySpawnerItem.h"
#include "World/Terrain/Terrain.h"
#include "Entities/Dynamic/Pawn.h"
#include "World/Camera.h"
#include "Game/Game.h"
#include "Entities/EntityData.h"
#include "Entities/Static/StaticEntity.h"

EntitySpwanerItem::EntitySpwanerItem(OverworldGame * game, const EntityID id) :
	m_game(game),
	m_entityID(id)
{
}

void EntitySpwanerItem::use(Pawn *player, const float delta)
{
	// Get block input position
	Vector2i blockPos = math::floor(m_game->getWorld()->getCamera()->getInputPosition() / BLOCK_PXF);

	// Set block if not occupied
	/*if(m_game->getWorld()->getTerrain()->setThingAt(blockPos.x, blockPos.y, m_entityID))
	{
		// Remove item from player inventory
		player->getCurrentItem()->dec();
	}*/

	EntityData *data = EntityData::get(m_entityID);
	if(data && data->getType() == STATIC_ENTITY)
	{
		((StaticEntityData*) EntityData::get(m_entityID))->create(m_game->getWorld(), blockPos.x, blockPos.y);
	}
}

void EntitySpwanerItem::draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha)
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