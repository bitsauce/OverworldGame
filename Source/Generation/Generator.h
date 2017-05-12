#pragma once

#include "Config.h"
#include "Constants.h"
#include "Blocks/Block.h"
#include "Formations/Formation.h"

class ChunkGenerator
{
public:
	ChunkGenerator(const uint seed);

	uint getSeed() const { return m_seed; }

	void getChunkBlocks(const int chunkX, const int chunkY, ChunkBlock *blocks);
	int getGroundHeight(const int x);

	//void setBlockAt(const int x, const int y, const WorldLayer z, const BlockID block);

private:
	void generateGroundHeight(const int chunkX);

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
	list<Formation*> m_formations;

	GraphicsContext *m_graphicsContext;
	RenderTarget2D *m_renderTarget;
	Resource<Shader> m_generationShader;

	RenderTarget2D *m_groundHeightRenderTarget;
	Resource<Shader> m_groundHeightShader;
	unordered_map<int, int*> m_groundHeight;
};