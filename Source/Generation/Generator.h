#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "Config.h"
#include "Constants.h"
#include "Noise/Simplex.h"
#include <unordered_set>
#include "Blocks/Block.h"

class ChunkGenerator
{
public:
	ChunkGenerator(const uint seed);

	uint getSeed() const { return m_seed; }
	Simplex2D &getSimplexNoise() { return m_noise; }
	Random getRandom() { return m_random; }

	void getChunkBlocks(const int chunkX, const int chunkY, ChunkBlock *blocks);
	int getGroundHeight(const int x);

	void setBlockAt(const int x, const int y, const WorldLayer z, const BlockID block);

private:
	void loadStructures(const int superChunkX, const int superChunkY);

	BlockID getGroundAt(const int x, const int y, const WorldLayer layer);

	uint m_seed;
	Simplex2D m_noise;

	//Resource<Shader> m_ge;

	Random m_random;

	map<int, BlockID*> m_chunkStructures;
};

#endif // WORLD_GENERATOR_H