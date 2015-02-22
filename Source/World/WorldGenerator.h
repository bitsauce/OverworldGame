#ifndef TERRAIN_GEN_H
#define TERRAIN_GEN_H

#include "Config.h"
#include "Constants.h"
#include "Structure.h"

enum BlockID;
enum TerrainLayer;

extern float step(float edge, float x);

#include "Noise/Simplex.h"
#include <unordered_set>

class WorldGenerator
{
public:

	void setSeed(const uint seed) { m_seed = seed; }
	uint getSeed() const { return m_seed; }

	void getChunkBlocks(const int chunkX, const int chunkY, BlockID *blocks);
	int getGroundHeight(const int x);
	//void loadStructures(const int x, const int y);

private:
	BlockID getGroundAt(const int x, const int y, const TerrainLayer layer);

	uint m_seed;
	Simplex2D m_noise;
	Random m_random;

	struct BlockUnion
	{
		BlockUnion() : back(BLOCK_EMPTY), middle(BLOCK_EMPTY), front(BLOCK_EMPTY) {}
		BlockID back;
		BlockID middle;
		BlockID front;
	};

	static map<int64_t, BlockUnion> s_structureMap;
	static unordered_set<uint> s_loadedSuperChunks;
};

#endif // TERRAIN_GEN_H