#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include "Config.h"

class BlockData
{
public:
	// Constructors
	BlockData(const BlockID id, const string &name, const Pixmap &pixmap, const ItemID item, const float opacity);
	
	// Data getters
	BlockID getID() const { return m_id; }
	ItemID getItem() const { return m_item; }
	Pixmap getPixmap() const { return m_pixmap; }
	float getOpacity() const { return m_opacity; }

	// Static functions
	static void init();
	static BlockData *get(const BlockID id);
	static BlockData *getByName(const string &name);

	static TextureAtlas *getTextureAtlas()
	{
		return s_textureAtlas;
	}

	static Resource<Texture2D> getDataTexture()
	{
		return s_dataTexture;
	}

	static map<BlockID, BlockData*>::const_iterator begin()
	{
		return s_data.cbegin();
	}
	
	static map<BlockID, BlockData*>::const_iterator end()
	{
		return s_data.cend();
	}
	
private:
	const string m_name;
	const BlockID m_id;
	const ItemID m_item;
	const Pixmap m_pixmap;
	const float m_opacity;

	// Static data
	static map<BlockID, BlockData*> s_data;
	static TextureAtlas *s_textureAtlas;
	static Resource<Texture2D> s_dataTexture;
};

#endif // BLOCK_DATA_H