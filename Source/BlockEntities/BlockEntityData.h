#pragma once

#include "Config.h"
#include "BlockEntity.h"
#include "Blocks/BlockData.h"

class World;

enum PlacementRule
{
	NEED_FLOOR = 1 << 1,
	NEED_WALL = 1 << 2,
	NEED_ROOF = 1 << 3,
	NEED_BACK_BLOCK = 1 << 4,
	NEED_ANY = NEED_FLOOR | NEED_WALL | NEED_ROOF | NEED_BACK_BLOCK
};

class BlockEntityData
{
	friend class OverworldGame;
public:
	BlockEntityData(const BlockEntityID id, const string &name, const Pixmap &pixmap, const int width, const int height, const uint frameCount, const WorldLayer layer, const uint placement, const function<BlockEntity*(World*, const int, const int, const BlockEntityData*)> factory) :
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

	int getWidth() const
	{
		return m_width;
	}

	int getHeight() const
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

	string getName() const
	{
		return m_name;
	}

	static const TextureAtlas *getTextureAtlas()
	{
		return s_textureAtlas;
	}

	static const Resource<Texture2D> getDataTexture()
	{
		return s_dataTexture;
	}

	bool isValidPlacement(const int x, const int y, class Terrain *terrain, BlockEntity *ignoreThis) const;

private:
	const BlockEntityID m_id;
	const string m_name;
	const Pixmap m_pixmap;
	const int m_width, m_height;
	const uint m_frameCount;
	const WorldLayer m_layer;
	const uint m_placement;
	const function<BlockEntity*(World*, const int, const int, const BlockEntityData*)> m_factory;

	static void init();

	// Static data
	static vector<BlockEntityData*> s_data;
	static TextureAtlas *s_textureAtlas;
	static Resource<Texture2D> s_dataTexture;
};