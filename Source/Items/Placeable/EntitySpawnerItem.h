#ifndef BLOCK_ENTITY_ITEM_H
#define BLOCK_ENTITY_ITEM_H

#include "Items/ItemData.h"

enum EntityID;

class EntitySpwanerItem : public ItemData
{
public:
	EntitySpwanerItem(Game *game, const EntityID id);

	void use(Pawn *player, const float delta);
	void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

private:
	Game *m_game;
	const EntityID m_entityID;
};

#endif // BLOCK_ENTITY_ITEM_H