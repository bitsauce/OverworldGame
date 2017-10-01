#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include "Config.h"
#include "Constants.h"
#include "Game/Storage.h"

class Pawn;
class World;
class Item;

struct ItemDataDesc
{
	const ItemID id;
	const string name;
	const string invName;
	const string desc;
	const string type;
	const string icon;
	const uint maxStack;
	const function<Item*(World*, Pawn*)> factory;
	const map<string, string> userData;
};

class Item
{
public:
	Item(World *world, Pawn *pawn) :
		m_world(world),
		m_pawn(pawn)
	{
	}

	virtual ~Item() { }

	virtual void equip() { }
	virtual void unequip() { }
	virtual void use(const float delta) { }
	virtual void update(const float delta) { }
	virtual void draw(SpriteBatch *spriteBatch, const float alpha) { }

protected:
	World *const m_world;
	Pawn *const m_pawn;
};

class ItemData
{
	friend class Overworld;
public:
	ItemData(const ItemDataDesc *);

	static ItemData *Get(const ItemID id)
	{
		return s_itemDataVector[id];
	}

	static ItemData *GetByName(const string &name)
	{
		map<string, ItemData*>::iterator itr = s_nameToData.find(name);
		if(itr == s_nameToData.end())
		{
			THROW("Could not find entity with name=%s", name.c_str());
		}
		return itr->second;
	}

	static Item *Create(const ItemID id, World *world, Pawn *pawn)
	{
		if(Get(id)->m_factory)
			return Get(id)->m_factory(world, pawn); else return 0;
	}

	static Item *CreateByName(const string &name, World *world, Pawn *pawn)
	{
		return GetByName(name)->m_factory(world, pawn);
	}

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
	function<Item*(World*, Pawn*)> m_factory;
	

	static void init(Overworld *game);
	static vector<ItemData*> s_itemDataVector;
	static map<string, ItemData*> s_nameToData;
};

#endif // ITEM_DATA_H