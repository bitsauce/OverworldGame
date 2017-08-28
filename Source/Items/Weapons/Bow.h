#ifndef BOW_H
#define BOW_H

#include "Config.h"
#include "Items/ItemData.h"

class Bow : public ItemData
{
public:
	Bow(Overworld *game, const ItemDataDesc *desc);
	void equip(Pawn *player);
	void unequip(Pawn *player);
	void update(Pawn *player, const float delta);
	void draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha);

private:
	Overworld * m_game;
	Sprite m_sprite;
	SpriteAnimation m_chargeAnim;
	Resource<Texture2D> m_attachTexture;
	bool m_charging;
	float m_chargeTime;
};

#endif // BOW_H