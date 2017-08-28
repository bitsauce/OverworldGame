#pragma once

#include "Config.h"
#include "BlockEntity.h"
#include "Blocks/BlockData.h"

class World;

typedef function<BlockEntity*(const Json::Value&)> BlockEntityFactory;

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
	friend class Overworld;
	friend class Terrain;
public:
	BlockEntityData(const BlockEntityID id, const string &name, const Pixmap &pixmap, const int width, const int height, const uint frameColumns, const uint frameRows, const WorldLayer layer, const uint placement, const BlockEntityFactory &factory) :
		m_pixmap(pixmap),
		m_id(id),
		m_name(name),
		m_width(width),
		m_height(height),
		m_frameColumns(frameColumns),
		m_frameRows(frameRows),
		m_layer(layer),
		m_placement(placement),
		m_factory(factory)
	{
		assert(m_name != "");
		assert(m_id >= 0);
		assert(m_width >= 0);
		assert(m_height >= 0);
		assert(m_frameColumns >= 0);
		assert(m_frameRows >= 0);
		assert(m_layer >= 0);
		assert(m_placement != 0);
		assert(m_factory);
	}

	static BlockEntityData *Get(const BlockEntityID id)
	{
		map<EntityID, BlockEntityData*>::iterator itr = s_idToData.find(id);
		if(itr == s_idToData.end())
		{
			THROW("Could not find block entity with id=%i", id);
		}
		return itr->second;
	}

	static BlockEntityData *GetByName(const string &name)
	{
		map<string, BlockEntityData*>::iterator itr = s_nameToData.find(name);
		if(itr == s_nameToData.end())
		{
			THROW("Could not find block entity with name=%s", name.c_str());
		}
		return itr->second;
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

	static const shared_ptr<Texture2D> getDataTexture()
	{
		return s_dataTexture;
	}

	uint getFrameColumns() const
	{
		return m_frameColumns;
	}

	uint getFrameRows() const
	{
		return m_frameRows;
	}

	bool isValidPlacement(const int x, const int y, class Terrain *terrain, BlockEntity *ignoreThis) const;

private:
	const BlockEntityID m_id;
	const string m_name;
	const Pixmap m_pixmap;
	const int m_width, m_height;
	const uint m_frameColumns;
	const uint m_frameRows;
	const WorldLayer m_layer;
	const uint m_placement;
	const BlockEntityFactory m_factory;

	static void init();

	// Static data
	static map<string, BlockEntityData*> s_nameToData;
	static map<BlockEntityID, BlockEntityData*> s_idToData;
	static TextureAtlas *s_textureAtlas;
	static shared_ptr<Texture2D> s_dataTexture;
};