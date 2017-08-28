#include "ItemDrop.h"
#include "Constants.h"
#include "Player.h"
#include "World/World.h"
#include "Items/ItemData.h"
#include "Entities/EntityData.h"

ItemDrop::ItemDrop(const Json::Value &attributes) :
	Entity(0, "ItemDrop", attributes),
	m_noPickupTime(1.0f), // No pickup for 1 s
	m_dragDistance(16.0f * BLOCK_PXF),
	m_pickupDistance(16.0f),
	m_hoverTime(0.0f),
	m_prevHoverTime(0.0f)
{
	if(attributes.isMember("item"))
	{
		const Json::Value &item = attributes["item"];
		m_itemID = item.get("id", ITEM_NONE).asInt();
		m_amount = item.get("amount", 1).asInt();
	}

	setGravityScale(0.1f);
	setSize(Vector2F(16.0f));
}

void ItemDrop::onTick(TickEvent *e)
{
	if(m_noPickupTime <= 0.0f)
	{
		list<Pawn*> pawns = m_world->getPawns();
		for(Pawn *pawn : pawns)
		{
			Vector2F deltaPosition = pawn->getCenter() - getCenter();
			if(pawn->getRect().contains(getCenter()))
			{
				m_amount -= m_amount - pawn->getStorage()->addItem(m_itemID, m_amount);
				if(m_amount <= 0)
				{
					delete this;
					return;
				}
			}
			else if(deltaPosition.length() <= m_dragDistance)
			{
				applyImpulse(deltaPosition.normalized());
			}
		}
	}
	m_noPickupTime -= e->getDelta();

	m_prevHoverTime = m_hoverTime;
	m_hoverTime += 5.0f * e->getDelta();

	Entity::onTick(e);
}

void ItemDrop::onDraw(DrawEvent *e)
{
	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	spriteBatch->drawSprite(Sprite(ItemData::get(m_itemID)->getIconTexture(), RectF(
		getDrawPosition(e->getAlpha()) - Vector2F(0.0f, ((sin(math::lerp(m_prevHoverTime, m_hoverTime, e->getAlpha())) * 0.5f + 0.5f) * 8.0f)) - Vector2F(4.0f, 4.0f),
		Vector2F(24.0f)),
		Vector2F(), 0.0f, TextureRegion())
		);
}