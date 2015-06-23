#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include "Config.h"
//>REMOVE ME<
enum BlockID;
enum ItemID;

class BlockData
{
public:
	// Constructors
	BlockData(BlockID id, const Texture2DPtr &texture, const ItemID item, const float opacity);
	
	// Data getters
	BlockID getID() const { return m_id; }
	ItemID getItem() const { return m_item; }
	Texture2DPtr getTexture() const { return m_texture; }
	float getOpacity() const { return m_opacity; }

	// Static functions
	static void init();
	static BlockData &get(const BlockID id);
	static TextureAtlas *getBlockAtlas() { return s_blockAtlas; }
	
private:
	BlockID m_id;
	ItemID m_item;
	Texture2DPtr m_texture;
	float m_opacity;

	// Static variables
	static vector<BlockData*> s_blockData;
	static TextureAtlas *s_blockAtlas;
};

#endif // BLOCK_DATA_H