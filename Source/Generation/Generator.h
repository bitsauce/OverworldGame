#ifndef TERRAIN_GEN_H
#define TERRAIN_GEN_H

#include "Config.h"
#include "Constants.h"
#include "Structure.h"
#include "Noise/Simplex.h"
#include <unordered_set>

enum BlockID;
enum TerrainLayer;

extern float step(float edge, float x);

class WorldGenerator
{
public:

	void setSeed(const uint seed) { m_seed = seed; }
	uint getSeed() const { return m_seed; }

	void getChunkBlocks(const int chunkX, const int chunkY, BlockID *blocks);
	int getGroundHeight(const int x);

private:
	void loadStructures(const int superChunkX, const int superChunkY);

	BlockID getGroundAt(const int x, const int y, const TerrainLayer layer);

	uint m_seed;
	Simplex2D m_noise;
	Random m_random;

	map<tuple<int, int, int>, BlockID> m_structureMap;
	unordered_set<uint> m_loadedSuperChunks;
};

#endif // TERRAIN_GEN_H