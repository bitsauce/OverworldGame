#include "Formation.h"

#define FORM_BLOCK_INDEX(x, y, cx, cy) ((((y) + (cy) * CHUNK_BLOCKS) * m_height) + ((x) + (cx) * CHUNK_BLOCKS))

Formation::Formation(const int width, const int height) :
	m_width(width),
	m_height(height)
{
}

bool Formation::existsAt(const int formX, const int formY) const
{
	return math::mod(formX + formY, 2) != 0; // TODO: Replace temp hash function
}

bool Formation::isGeneratedAt(const int formX, const int formY) const
{
	return m_blocks.find(CHUNK_KEY(formX, formY)) != m_blocks.end();
}

void Formation::generateFormationBlocks(const int formX, const int formY)
{
	BlockID *formationBlocks = new BlockID[m_width * m_height * CHUNK_BLOCKS * CHUNK_BLOCKS];
	generate(formX, formY, formationBlocks);
	m_blocks[CHUNK_KEY(formX, formY)] = formationBlocks;
}

void Formation::getChunkBlocks(const int formX, const int formY, const int chunkOffsetX, const int chunkOffsetY, ChunkBlock *blocks)
{
	const BlockID *blockIDs = m_blocks[CHUNK_KEY(formX, formY)];
	for(int y = 0; y < CHUNK_BLOCKS; y++)
	{
		for(int x = 0; x < CHUNK_BLOCKS; x++)
		{
			const BlockID blockID = blockIDs[FORM_BLOCK_INDEX(x, y, chunkOffsetX, chunkOffsetY)];
			if(blockID != 10)
			{
				blocks[BLOCK_INDEX(x, y, 0)].setBlockData(BlockData::get(blockID));
			}
		}
	}
}
