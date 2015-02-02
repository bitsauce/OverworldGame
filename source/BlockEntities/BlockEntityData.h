#ifndef BLOCK_ENTITY_DATA_H
#define BLOCK_ENTITY_DATA_H

#include "Config.h"
#include "Constants.h"

class Terrain;
class BlockEntity;

class BlockEntityData
{
public:
	BlockEntityData(const int width, const int height, function<void(int, int)> factory);

	bool tryPlace(const int x, const int y);

	static void init(Terrain *terrain);
	static BlockEntityData &get(const BlockEntityID id);

private:
	const int m_width, m_height;
	const function<void(int, int)> m_factory;

	static Terrain *s_terrain;
	static vector<BlockEntityData*> s_blockEntityData;
};

#endif // BLOCK_ENTITY_DATA_H