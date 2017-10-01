#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include "Config.h"

class ItemData;

class BlockData
{
	friend class InGameDebug;
public:
	// Constructor
	BlockData(const BlockID id, const string &name, const Pixmap &pixmap, const string &item, const float opacity, const bool solid);
	
	// Getters
	BlockID getID() const { return m_id; }
	string getName() const { return m_name; }
	ItemData *getItem() const;
	Pixmap getPixmap() const { return m_pixmap; }
	float getOpacity() const { return m_opacity; }
	bool isSolid() const { return m_solid; }

	// Static functions
	static void init();
	static BlockData *get(const BlockID id);
	static BlockData *get(const string &name);

	static TextureAtlas *getTextureAtlas()
	{
		return s_textureAtlas;
	}

	static shared_ptr<Texture2D> getDataTexture()
	{
		return s_dataTexture;
	}
	
private:
	const string m_name;
	const BlockID m_id;
	const string m_itemName;
	const Pixmap m_pixmap;
	const float m_opacity;
	const bool m_solid;

	// Static data
	static vector<BlockData*> s_blockDataVector;
	static map<string, BlockData*> s_nameToData;
	static TextureAtlas *s_textureAtlas;
	static shared_ptr<Texture2D> s_dataTexture;
};

#endif // BLOCK_DATA_H