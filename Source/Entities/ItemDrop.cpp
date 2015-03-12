#include "ItemDrop.h"
#include "Constants.h"
#include "Player.h"
#include "World/World.h"
#include "Items/ItemData.h"

ItemDrop::ItemDrop(World &world, const Vector2 position, const ItemID item) :
	DynamicEntity(world, ENTITY_ITEM_DROP),
	m_itemID(item),
	m_dragDistance(16.0f * BLOCK_PXF),
	m_pickupDistance(16.0f)
{
	setPosition(position);
	Entity::setSize(16.f, 16.f);
}

void ItemDrop::update(const float dt)
{
	m_prevPosition = getPosition();

	list<Player*> players = m_world.getPlayers();
	for(Player *player : players)
	{
		Vector2 deltaPosition = player->getCenter() - getCenter();
		if(player->getRect().contains(getCenter()))
		{
			player->getItemContainer().addItem(m_itemID);
			delete this;
			return;
		}
		else if(deltaPosition.magnitude() <= m_dragDistance)
		{
			applyImpulse(deltaPosition.normalized());
		}
	}

	update(dt);
}

void ItemDrop::draw(SpriteBatch *spriteBatch, const float alpha)
{
	spriteBatch->drawSprite(Sprite(ItemData::get(m_itemID)->getIconTexture(), Rect(math::lerp(m_prevPosition, getPosition(), alpha), getSize())));
}