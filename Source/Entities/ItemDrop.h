#ifndef ITEM_DROP_H
#define ITEM_DROP_H

#include "Config.h"
#include "Entities/Entity.h"
#include "Entities/Physics/DynamicEntity.h"

enum ItemID;

class ItemDrop : public DynamicEntity
{
public:
	ItemDrop(World *world, const Vector2 position, const ItemID item);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

private:
	const ItemID m_itemID;
	Vector2 m_prevPosition;
	float m_dragDistance;
	float m_pickupDistance;
	float m_hoverTime;
	float m_prevHoverTime;
};

#endif // ITEM_DROP_H