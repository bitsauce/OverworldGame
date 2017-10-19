#pragma once

#include "Config.h"
#include "Items/ItemData.h"

class Bow : public Item
{
public:
	Bow(World *world, Pawn *pawn);
	void equip();
	void unequip();
	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	static Item *Factory(World *world, Pawn *pawn) { return new Bow(world, pawn); }

private:
	Sprite m_sprite;
	SpriteAnimation m_chargeAnim;
	Resource<Texture2D> m_attachTexture;
	bool m_charging;
	float m_chargeTime;
};
