#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include "Config.h"
#include "Game/Storage.h"

class Pawn;
enum ItemID;

class ItemData
{
	friend class Game;
public:
	ItemData();
	virtual void equip(Pawn *pawn) { }
	virtual void unequip(Pawn *pawn) { }
	virtual void use(Pawn *pawn, const float delta) { }
	virtual void update(Pawn *pawn, const float delta) { }
	virtual void draw(Pawn *pawn, SpriteBatch *spriteBatch, const float alpha) { }
	static ItemData *get(const ItemID id) { return s_itemData[id]; }

	ItemID getID() const { return m_id; }
	Texture2DPtr getIconTexture() const { return m_iconTexture; }
	uint getMaxStack() const { return m_maxStack; }

protected:
	virtual void set(const ItemID id, const string &name, const string &desc, const string &texture, const uint maxStack);

private:
	ItemID m_id;
	string m_name;
	string m_desc;
	uint m_maxStack;
	Texture2DPtr m_iconTexture;

	static void init(Game *game);
	static vector<ItemData*> s_itemData;
};

#endif // ITEM_DATA_H