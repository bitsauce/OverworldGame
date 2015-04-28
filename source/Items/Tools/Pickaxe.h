#ifndef PICKAXE_H
#define PICKAXE_H

#include "Config.h"
#include "Items/ItemData.h"

class Pickaxe : public ItemData
{
public:
	Pickaxe(Game *game);
	void use(ItemContainer::Slot *slot, const float delta);
	void draw(Player *player, SpriteBatch *spriteBatch, const float alpha);

private:
	Game *m_game;
	Sprite m_cracksSprite;
	SpriteAnimation m_cracksAnimation;
	Sprite m_sprite;
	Vector2i m_prevBlockPosition;
	float m_mineCounter;
	float m_mineTime;
};

#endif // PICKAXE_H