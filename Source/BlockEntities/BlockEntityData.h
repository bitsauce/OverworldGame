#pragma once

#include "Config.h"
#include "BlockEntity.h"
#include "Torch.h"

class World;

class BlockEntityData
{
	friend class OverworldGame;
public:
	BlockEntityData(const BlockEntityID id, const string &name, const Pixmap &pixmap, const uint width, const uint height, const function<BlockEntity*(World*, const int, const int, const BlockEntityData*)> factory) :
		m_id(id),
		m_name(name),
		m_pixmap(pixmap),
		m_width(width),
		m_height(height),
		m_factory(factory)
	{
	}

	static BlockEntityData *get(const BlockEntityID id)
	{
		if(!s_data[id])
		{
			THROW("BlockEntityData (id=%i) has no BlockEntityData", id);
		}
		return s_data[id];
	}

	static BlockEntityData *getByName(const string &name)
	{
		for(int i = 0; i < ENTITY_COUNT; ++i)
		{
			if(s_data[i] && s_data[i]->m_name == name)
			{
				return s_data[i];
			}
		}
		return 0;
	}

	BlockEntity *create(World *world, const int x, const int y)
	{
		if(!m_factory)
		{
			THROW("Entity (id=%i) has no factory", getID());
		}
		return m_factory(world, x, y, this);
	}


	const BlockEntityID getID() const
	{
		return m_id;
	}

private:
	const BlockEntityID m_id;
	const string &m_name;
	const function<BlockEntity*(World*, const int, const int, const BlockEntityData*)> m_factory;
	const uint m_width, m_height;
	const Pixmap m_pixmap;

	static void init();
	static vector<BlockEntityData*> s_data;
	static TextureAtlas *s_atlas;
};