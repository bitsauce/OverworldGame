#ifndef ITEM_DROP_H
#define ITEM_DROP_H

#include "Config.h"
#include "Game/GameObject.h"
#include "Physics/PhysicsBody.h"

enum ItemID;

class ItemDrop : public GameObject
{
public:
	ItemDrop(const Vector2 position, const ItemID item);

	void update();
	void draw(SpriteBatch *spriteBatch);

private:
	const ItemID m_itemID;
	PhysicsBody m_body;
	float m_dragDistance;
	float m_pickupDistance;
};

#endif // ITEM_DROP_H