#include "ItemDrop.h"
#include "Constants.h"
#include "Player.h"
#include "World/World.h"
#include "Items/ItemData.h"

ItemDrop::ItemDrop(World &world, const Vector2 position, const ItemID item) :
	Entity(world, ENTITY_ITEM_DROP),
	m_itemID(item),
	m_body(world),
	m_dragDistance(16.0f * BLOCK_PXF),
	m_pickupDistance(16.0f)
{
	m_body.setPosition(position.x, position.y);
	m_body.setSize(16.f, 16.f);
}

void ItemDrop::update()
{
	list<Player*> players = m_world.getPlayers();
	for(Player *player : players)
	{
		Vector2 deltaPosition = player->getBody().getCenter() - m_body.getCenter();
		if(player->getBody().getRect().contains(m_body.getCenter()))
		{
			player->getItemContainer().addItem(m_itemID);
			delete this;
			return;
		}
		else if(deltaPosition.magnitude() <= m_dragDistance)
		{
			m_body.applyImpulse(deltaPosition.normalized());
		}
	}

	m_body.update();
}

void ItemDrop::draw(SpriteBatch *spriteBatch)
{
	spriteBatch->drawSprite(Sprite(ItemData::get(m_itemID)->getIconTexture(), Rect(m_body.getPosition(), m_body.getSize())));
}