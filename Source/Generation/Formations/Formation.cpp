#include "Formation.h"

Formation::Formation(const int width, const int height) :
	m_width(width),
	m_height(height)
{
}

Formation::~Formation()
{
}

bool Formation::isGeneratedAt(const int formX, const int formY) const
{
	return m_generated.find(CHUNK_KEY(formX, formY)) != m_generated.end();
}

void Formation::generate(const int formX, const int formY)
{
	list<FormationElement*> elements;
	generate(formX, formY, elements);
	for(FormationElement *elem : elements)
	{
		int x1 = math::floor(elem->m_x / CHUNK_BLOCKSF),
			y1 = math::floor(elem->m_y / CHUNK_BLOCKSF),
			x2 = math::floor((elem->m_x + elem->m_w - 1) / CHUNK_BLOCKSF),
			y2 = math::floor((elem->m_y + elem->m_h - 1) / CHUNK_BLOCKSF);
		for(int y = y1; y <= y2; y++)
		{
			for(int x = x1; x <= x2; x++)
			{
				m_chunkToElementListMap[CHUNK_KEY(x, y)].push_back(elem);
			}
		}
	}
	m_generated.insert(CHUNK_KEY(formX, formY));
}

void Formation::getChunkBlocks(const int chunkX, const int chunkY, ChunkBlock *blocks)
{
	list<FormationElement*> elements = m_chunkToElementListMap[CHUNK_KEY(chunkX, chunkY)];
	for(FormationElement *elem : elements)
	{
		int x1 = math::mod(max(elem->m_x, chunkX * CHUNK_BLOCKS), CHUNK_BLOCKS),
			y1 = math::mod(max(elem->m_y, chunkY * CHUNK_BLOCKS), CHUNK_BLOCKS),
			x2 = math::mod(min(elem->m_x + elem->m_w, (chunkX + 1) * CHUNK_BLOCKS) - 1, CHUNK_BLOCKS),
			y2 = math::mod(min(elem->m_y + elem->m_h, (chunkY + 1) * CHUNK_BLOCKS) - 1, CHUNK_BLOCKS);
		for(int y = y1; y <= y2; y++)
		{
			for(int x = x1; x <= x2; x++)
			{
				blocks[BLOCK_INDEX(x, y, 0)].setBlockData(BlockData::get(elem->m_blocks[(y - y1) * elem->m_w + (x - x1)]));
			}
		}
	}
	m_chunkToElementListMap.erase(CHUNK_KEY(chunkX, chunkY)); // NOTE: FormationElements which are no longer needed should be deleted. Memory leak!
}
