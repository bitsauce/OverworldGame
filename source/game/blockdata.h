#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include <x2d/x2d.h>

enum BlockID;
enum ItemID;
class b2Fixture;

class BlockData
{
public:
	BlockData();
	BlockData(BlockID id, const xd::Texture2DPtr &texture, const ItemID item, const float opacity);
	
	void setupFixture(b2Fixture *fixture) {}

	xd::Texture2DPtr getTexture() const { return m_texture; }
	float getOpacity() const { return m_opacity; }

	static void init();
	static BlockData &get(const BlockID id);
	static xd::TextureAtlas *getBlockAtlas() { return s_blockAtlas; }
	
private:
	BlockID m_id;
	xd::Texture2DPtr m_texture;
	ItemID m_item;
	float m_opacity;

	static vector<BlockData> s_blockData;
	static xd::TextureAtlas *s_blockAtlas;
};

#endif // BLOCK_DATA_H