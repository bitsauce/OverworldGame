#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H

#include "Config.h"
#include "Constants.h"
#include <unordered_set>
#include "Blocks/Block.h"

class ChunkGenerator
{
public:
	ChunkGenerator(const uint seed);

	uint getSeed() const { return m_seed; }

	void getChunkBlocks(const int chunkX, const int chunkY, ChunkBlock *blocks);
	int getGroundHeight(const int x);

	void setBlockAt(const int x, const int y, const WorldLayer z, const BlockID block);

private:

	enum BlockType
	{
		BLOCK_EMPTY,
		BLOCK_GRASS,
		BLOCK_STONE,
		BLOCK_DIRT_BACK,
		BLOCK_COUNT
	};

	const uint m_seed;
	const BlockData *m_blockData[BLOCK_COUNT];

	GraphicsContext *m_graphicsContext;
	Resource<RenderTarget2D> m_renderTarget;
	Resource<Shader> m_generationShader;
};

#endif // WORLD_GENERATOR_H