#ifndef TERRAIN_GEN_H
#define TERRAIN_GEN_H

#include "Config.h"
#include "Constants.h"
#include "Noise/Simplex.h"
#include <unordered_set>
//>REMOVE ME<
extern float step(float edge, float x);

class WorldGenerator
{
public:
	WorldGenerator(const uint seed) :
		m_seed(seed)
	{
		m_random.setSeed(seed);
	}

	uint getSeed() const { return m_seed; }
	Simplex2D &getSimplexNoise() { return m_noise; }
	Random getRandom() { return m_random; }

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

class StructurePlacer
{
	friend class WorldGenerator;
public:
	void setBlockAt(const int x, const int y, const TerrainLayer z, const BlockID block);

private:
	StructurePlacer(map<tuple<int, int, int>, BlockID> *sm) :
		m_structureMap(sm)
	{
	}

	map<tuple<int, int, int>, BlockID> *m_structureMap;
};

#endif // TERRAIN_GEN_H