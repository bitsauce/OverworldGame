#ifndef BLOCK_DATA_H
#define BLOCK_DATA_H

#include <x2d/x2d.h>

enum BlockID;
class b2Fixture;

class BlockData
{
	friend class TerrainChunk;
public:
	BlockData();
	BlockData(BlockID id, const shared_ptr<XTexture> &texture, const /*ItemID*/ uint i, const float opacity);
	
	void setupFixture(b2Fixture *fixture) {}

	shared_ptr<XTexture> getTexture() const { return m_texture; }

	static void init();
	static BlockData &get(const BlockID id);
	
private:
	BlockID m_id;
	shared_ptr<XTexture> m_texture;
	uint m_itemID;
	float m_opacity;

	static vector<BlockData> s_blockData;
	static XTextureAtlas *s_blockAtlas;
};

#endif // BLOCK_DATA_H