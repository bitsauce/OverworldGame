#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include "Config.h"
#include "Constants.h"
#include "Game/Storage.h"

class Pawn;
enum ItemID;

class ItemData
{
	friend class OverworldGame;
public:
	ItemData();
	virtual void equip(Pawn *pawn) { }
	virtual void unequip(Pawn *pawn) { }
	virtual void use(Pawn *pawn, const float delta) { }
	virtual void update(Pawn *pawn, const float delta) { }
	virtual void draw(Pawn *pawn, SpriteBatch *spriteBatch, const float alpha) { }
	static ItemData *get(const ItemID id) { return s_data[id]; }


	static ItemData *getByName(const string &name)
	{
		for(int i = 0; i < ITEM_COUNT; ++i)
		{
			if(s_data[i] && s_data[i]->m_name == name)
			{
				return s_data[i];
			}
		}
		return 0;
	}

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

	static void init(OverworldGame *game);
	static vector<ItemData*> s_data;
};

#endif // ITEM_DATA_H