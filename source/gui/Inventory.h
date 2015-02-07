#ifndef INVENTORY_H
#define INVENTORY_H

#include "Config.h"
#include "UiObject.h"

class ItemContainer;

class Inventory : public UiObject, public xd::MouseListener
{
public:
	Inventory(UiObject *object, ItemContainer *itemContainer);

	void update();
	void draw(SpriteBatch *spriteBatch);

private:

	ItemContainer *m_itemContainer;
	Sprite *m_sprite;
};

#endif // INVENTORY_H