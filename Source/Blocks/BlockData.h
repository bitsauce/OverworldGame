#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include "Config.h"

#define BLOCK_CONCAT_ID(BLOCKID, SUBBLOCKID) ((BLOCKID & 0x0FFFFFFF) | (SUBBLOCKID << 28))

class ItemData;

class BlockData
{
	friend class Debug;
public:
	// Constructor
	BlockData(const BlockID id, const string &name, const map<uint, Pixmap*> pixmaps, const string &item, const float opacity, const bool solid);
	
	// Getters
	BlockID getID() const { return m_id; }
	string getName() const { return m_name; }
	ItemData *getItem() const;
	const Pixmap *getPixmap(const uint subId) const;
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
	const map<uint, Pixmap*> m_pixmaps;
	const float m_opacity;
	const bool m_solid;

	// Static data
	static map<BlockID, BlockData*> s_idToData;
	static map<string, BlockData*> s_nameToData;
	static TextureAtlas *s_textureAtlas;
	static shared_ptr<Texture2D> s_dataTexture;
};

#endif // BLOCK_DATA_H