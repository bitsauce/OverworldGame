#ifndef ITEM_DROP_H
#define ITEM_DROP_H

#include "Config.h"
#include "Game/Game.h"
#include "Entity.h"

class ItemDrop : public Entity
{
public:
	ItemDrop(const Json::Value &attributes);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	static Entity *Factory(const Json::Value &attributes)
	{
		return new ItemDrop(attributes);
	}

private:
	ItemID m_itemID;
	int m_amount;
	float m_noPickupTime;
	float m_dragDistance;
	float m_pickupDistance;
	float m_hoverTime;
	float m_prevHoverTime;
};

#endif // ITEM_DROP_H