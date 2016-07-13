#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include "Config.h"

class BlockData
{
	friend class Debug;
public:
	// Constructor
	BlockData(const BlockID id, const string &name, const Pixmap &pixmap, const ItemID item, const float opacity);
	
	// Getters
	BlockID getID() const { return m_id; }
	string getName() const { return m_name; }
	ItemID getItem() const { return m_item; }
	Pixmap getPixmap() const { return m_pixmap; }
	float getOpacity() const { return m_opacity; }

	// Static functions
	static void init();
	static BlockData *get(const BlockID id);
	static BlockData *get(const string &name);

	static TextureAtlas *getTextureAtlas()
	{
		return s_textureAtlas;
	}

	static Resource<Texture2D> getDataTexture()
	{
		return s_dataTexture;
	}
	
private:
	const string m_name;
	const BlockID m_id;
	const ItemID m_item;
	const Pixmap m_pixmap;
	const float m_opacity;

	// Static data
	static map<BlockID, BlockData*> s_idToData;
	static map<string, BlockData*> s_nameToData;
	static TextureAtlas *s_textureAtlas;
	static Resource<Texture2D> s_dataTexture;
};

#endif // BLOCK_DATA_H