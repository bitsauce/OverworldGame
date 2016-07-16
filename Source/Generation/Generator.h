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

	Resource<Shader> m_generationShader;

	enum BlockType
	{
		BLOCK_EMPTY,
		BLOCK_GRASS,
		BLOCK_STONE,
		BLOCK_DIRT_BACK,
		BLOCK_COUNT
	};

	BlockType getGroundAt(const int x, const int y, const WorldLayer layer);

	uint m_seed;
	Simplex2D m_noise;
	Random m_random;

	const BlockData *m_blockData[BLOCK_COUNT];

	map<int, BlockID*> m_chunkStructures;

	GraphicsContext *m_graphicsContext;
	Resource<RenderTarget2D> m_renderTarget;
};

#endif // WORLD_GENERATOR_H