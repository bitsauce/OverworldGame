#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include "Config.h"
#include "Constants.h"
#include "Game/Storage.h"

class Pawn;

struct ItemDataDesc
{
	const ItemID id;
	const string name;
	const string invName;
	const string desc;
	const string type;
	const string icon;
	const uint maxStack;
	const map<string, string> userData;
};

class ItemData
{
	friend class OverworldGame;
public:
	ItemData(const ItemDataDesc *);
	
	virtual void equip(Pawn *pawn) { }
	virtual void unequip(Pawn *pawn) { }
	virtual void use(Pawn *pawn, const float delta) { }
	virtual void update(Pawn *pawn, const float delta) { }
	virtual void draw(Pawn *pawn, SpriteBatch *spriteBatch, const float alpha) { }

	static ItemData *get(const ItemID id) { return s_data[id]; }
	static ItemData *getByName(const string &name);

	ItemID getID() const { return m_id; }
	Resource<Texture2D> getIconTexture() const { return m_iconTexture; }
	uint getMaxStack() const { return m_maxStack; }
	string getName() const { return m_name; }
	string getDesc() const { return m_desc; }

private:
	ItemID m_id;
	string m_name;
	string m_desc;
	uint m_maxStack;
	Resource<Texture2D> m_iconTexture;
	//map<string, string> m_userData;

	static void init(OverworldGame *game);
	static map<ItemID, ItemData*> s_data;
};

#endif // ITEM_DATA_H