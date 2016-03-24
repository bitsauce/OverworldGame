#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include "Config.h"

enum BlockID;
enum ItemID;

class BlockData
{
public:
	// Constructors
	BlockData(BlockID id, const Pixmap &pixmap, const ItemID item, const float opacity);
	
	// Data getters
	BlockID getID() const { return m_id; }
	ItemID getItem() const { return m_item; }
	Pixmap getPixmap() const { return m_pixmap; }
	float getOpacity() const { return m_opacity; }

	// Static functions
	static void init();
	static BlockData *get(const BlockID id);

	static TextureAtlas *getTextureAtlas()
	{
		return s_textureAtlas;
	}

	static Resource<Texture2D> getDataTexture()
	{
		return s_dataTexture;
	}
	
private:
	const BlockID m_id;
	const ItemID m_item;
	const Pixmap m_pixmap;
	const float m_opacity;

	// Static data
	static vector<BlockData*> s_data;
	static TextureAtlas *s_textureAtlas;
	static Resource<Texture2D> s_dataTexture;
};

#endif // BLOCK_DATA_H