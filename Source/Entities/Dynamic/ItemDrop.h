#ifndef ITEM_DROP_H
#define ITEM_DROP_H

#include "Config.h"
#include "Game/Game.h"
#include "DynamicEntity.h"

enum ItemID;

class ItemDrop : public DynamicEntity
{
public:
	ItemDrop(World *world, const ItemID item, const int amount = 1);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	static DynamicEntity *Factory(World * world)
	{
		return new ItemDrop(world, ITEM_NONE, 1);
	}

private:
	const ItemID m_itemID;
	int m_amount;
	float m_noPickupTime;
	float m_dragDistance;
	float m_pickupDistance;
	float m_hoverTime;
	float m_prevHoverTime;
};

#endif // ITEM_DROP_H