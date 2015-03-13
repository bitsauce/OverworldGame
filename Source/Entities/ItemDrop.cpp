#include "ItemDrop.h"
#include "Constants.h"
#include "Player.h"
#include "World/World.h"
#include "Items/ItemData.h"

ItemDrop::ItemDrop(World &world, const Vector2 position, const ItemID item) :
	DynamicEntity(world, ENTITY_ITEM_DROP),
	m_itemID(item),
	m_dragDistance(16.0f * BLOCK_PXF),
	m_pickupDistance(16.0f),
	m_prevPosition(position),
	m_hoverTime(0.0f),
	m_prevHoverTime(0.0f)
{
	setGravityScale(0.1f);
	setPosition(position);
	setSize(Vector2(16.0f));
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

	m_prevHoverTime = m_hoverTime;
	m_hoverTime += 5.0f * dt;

	DynamicEntity::update(dt);
}

void ItemDrop::draw(SpriteBatch *spriteBatch, const float alpha)
{
	spriteBatch->drawSprite(Sprite(ItemData::get(m_itemID)->getIconTexture(), Rect(
		math::lerp(m_prevPosition, getPosition(), alpha) - Vector2(0.0f, ((sin(math::lerp(m_prevHoverTime, m_hoverTime, alpha)) * 0.5f + 0.5f) * 8.0f)),
		getSize()),
		Vector2(), 0.0f, TextureRegion(4.0f / 24.0f, 4.0f / 24.0f, 1.0f - 4.0f / 24.0f, 1.0f - 4.0f / 24.0f))
		);
}