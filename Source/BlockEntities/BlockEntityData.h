#pragma once

#include "Config.h"
#include "BlockEntity.h"
#include "Blocks/BlockData.h"

class World;

class BlockEntityData
{
	friend class OverworldGame;
public:
	BlockEntityData(const BlockEntityID id, const string &name, const Pixmap &pixmap, const uint width, const uint height, const uint frameCount, const WorldLayer layer, const uint placement, const function<BlockEntity*(World*, const int, const int, const BlockEntityData*)> factory) :
		m_pixmap(pixmap),
		m_id(id),
		m_name(name),
		m_width(width),
		m_height(height),
		m_frameCount(frameCount),
		m_layer(layer),
		m_placement(placement),
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
		for(int i = 0; i < BLOCK_ENTITY_COUNT; ++i)
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

	uint getWidth() const
	{
		return m_width;
	}

	uint getHeight() const
	{
		return m_height;
	}

	Pixmap getPixmap() const
	{
		return m_pixmap;
	}

	WorldLayer getLayer() const
	{
		return m_layer;
	}

	uint getPlacement() const
	{
		return m_placement;
	}

	bool canPlace(const int x, const int y, const enum WorldLayer layer, class Terrain *terrain) const;

private:
	const BlockEntityID m_id;
	const string m_name;
	const Pixmap m_pixmap;
	const uint m_width, m_height;
	const uint m_frameCount;
	const WorldLayer m_layer;
	const uint m_placement;
	const function<BlockEntity*(World*, const int, const int, const BlockEntityData*)> m_factory;

	static void init();
	static vector<BlockEntityData*> s_data;
public:
	static TextureAtlas *s_atlas; // TODO: Should not be public
	static Resource<Texture2D> s_blockEntityDataTexture;
};