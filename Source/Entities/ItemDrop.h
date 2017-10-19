#pragma once

#include "Config.h"
#include "Game/Game.h"
#include "Entity.h"

class ItemDrop : public Entity
{
	DECLARE_ENTITY(ItemDrop)
public:
	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

private:
	ItemID m_itemID;
	int m_amount;
	float m_noPickupTime;
	float m_dragDistance;
	float m_pickupDistance;
	float m_hoverTime;
	float m_prevHoverTime;
};
