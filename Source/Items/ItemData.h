#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include "Config.h"
#include "Constants.h"
#include "Game/Storage.h"

class Pawn;
class World;
class Item;

typedef function<Item*(World*, Pawn*, const Json::Value&)> ItemFactory;

struct ItemDataDesc
{
	const ItemID id;
	const string name;
	const string invName;
	const string desc;
	const string icon;
	const uint maxStack;
	const ItemFactory factory;
	const Json::Value &attributes;
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
		if(id < 0 && id >= s_itemDataVector.size())
		{
			THROW("Could not find item with id=%i", id);
		}
		return s_itemDataVector[id];
	}

	static ItemData *GetByName(const string &name)
	{
		map<string, ItemData*>::iterator itr = s_nameToData.find(name);
		if(itr == s_nameToData.end())
		{
			THROW("Could not find item \"%s\"", name.c_str());
		}
		return itr->second;
	}

	static Item *Create(const ItemID id, World *world, Pawn *pawn)
	{
		ItemData *data = Get(id);
		if(data->m_factory)
		{
			return data->m_factory(world, pawn, data->m_attributes);
		}
		return 0;
	}

	static Item *CreateByName(const string &name, World *world, Pawn *pawn)
	{
		ItemData *data = GetByName(name);
		if(data->m_factory)
		{
			return data->m_factory(world, pawn, data->m_attributes);
		}
		return 0;
	}

	ItemID getID() const { return m_id; }
	Resource<Texture2D> getIconTexture() const { return m_iconTexture; }
	uint getMaxStack() const { return m_maxStack; }
	string getName() const { return m_name; }
	string getDesc() const { return m_desc; }

private:
	const ItemID m_id;
	const string m_name;
	const string m_desc;
	const uint m_maxStack;
	const Resource<Texture2D> m_iconTexture;
	const Json::Value m_attributes;
	const ItemFactory m_factory;
	

	static void init(Overworld *game);
	static vector<ItemData*> s_itemDataVector;
	static map<string, ItemData*> s_nameToData;
};

#endif // ITEM_DATA_H