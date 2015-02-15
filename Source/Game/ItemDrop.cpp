#include "ItemDrop.h"
#include "Constants.h"
#include "Game.h"

ItemDrop::ItemDrop(const Vector2 position, const ItemID item) :
	GameObject(DRAW_ORDER_ITEM_DROP),
	m_itemID(item),
	m_body(),
	m_dragDistance(16.0f * BLOCK_PXF),
	m_pickupDistance(16.0f)
{
	m_body.setPosition(position.x, position.y);
	m_body.setSize(16.f, 16.f);
}

void ItemDrop::update()
{
	vector<Player*> players = World::getPlayers();
	for(uint i = 0; i < players.size(); ++i)
	{
		Player *player = players[i];
		Vector2 deltaPosition = player->getBody()->getCenter() - m_body.getCenter();
		if(player->getBody()->getRect().contains(m_body.getCenter()))
		{
			players[i]->getItemContainer().addItem(m_itemID);
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