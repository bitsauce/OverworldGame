#include "Inventory.h"
#include "Items/ItemContainer.h"

Inventory::Inventory(UiObject *parent, ItemContainer *itemContainer) :
	UiObject(parent),
	m_itemContainer(itemContainer),
	m_sprite(new Sprite(ResourceManager::get<Texture2D>(":/Sprites/Inventory/item_slot.png")))
{
	m_sprite->setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
}

void Inventory::update()
{
}

void Inventory::draw(SpriteBatch *spriteBatch)
{
	for(uint i = 0; i < m_itemContainer->getSize(); ++i)
	{
		m_sprite->setPosition(5 + i*34, 5);
		spriteBatch->drawSprite(*m_sprite);
	}
}