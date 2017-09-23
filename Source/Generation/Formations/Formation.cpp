#include "Formation.h"

#include "Game/Game.h"

Formation::Formation(ChunkGenerator *gen, const int width, const int height) :
	m_width(width),
	m_height(height),
	m_xorhash(gen->getSeed())
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
		int x1 = math::floor(elem->m_x1 / CHUNK_BLOCKSF),
			y1 = math::floor(elem->m_y1 / CHUNK_BLOCKSF),
			x2 = math::floor((elem->m_x2) / CHUNK_BLOCKSF),
			y2 = math::floor((elem->m_y2) / CHUNK_BLOCKSF);
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

void Formation::getBlocks(const int chunkX, const int chunkY, BlockID *blocks)
{
	list<FormationElement*> elements = m_chunkToElementListMap[CHUNK_KEY(chunkX, chunkY)];
	for(FormationElement *elem : elements)
	{
		int x1 = max(elem->m_x1, chunkX * CHUNK_BLOCKS),
			y1 = max(elem->m_y1, chunkY * CHUNK_BLOCKS),
			x2 = min(elem->m_x2, (chunkX + 1) * CHUNK_BLOCKS - 1),
			y2 = min(elem->m_y2, (chunkY + 1) * CHUNK_BLOCKS - 1);
		for(int y = y1; y <= y2; y++)
		{
			for(int x = x1; x <= x2; x++)
			{
				unordered_map<uint32_t, FormationElement::Block[WORLD_LAYER_COUNT]>::const_iterator itr = elem->m_blocks.find(CHUNK_KEY(x, y));
				if(itr != elem->m_blocks.end())
				{
					for(int layer = 0; layer < WORLD_LAYER_COUNT; layer++)
					{
						const FormationElement::Block &block = itr->second[layer];
						BlockID &blockToReplace = blocks[BLOCK_INDEX(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer)];
						if(block.replace == FormationElement::REPLACE_ALWAYS || (block.replace == FormationElement::REPLACE_IF_EMPTY && blockToReplace == 0))
						{
							blockToReplace = block.blockID;
						}
						//blockToReplace.setBlockEntity(block.blockEntityID);
					}
				}
			}
		}
	}
	m_chunkToElementListMap.erase(CHUNK_KEY(chunkX, chunkY)); // NOTE: FormationElements which are no longer needed should be deleted. Memory leak!
}

void FormationElement::setBlockAt(const int x, const int y, const WorldLayer layer, const BlockID id, const ReplacePolicy replace)
{
	// Expand bounding box
	if(x < 0) m_x1 = min(m_x1, m_originX + x);
	else m_x2 = max(m_x2, m_originX + x);
	if(y < 0) m_y1 = min(m_y1, m_originY + y);
	else m_y2 = max(m_y2, m_originY + y);

	// Set block at position
	Block &block = m_blocks[CHUNK_KEY(m_originX + x, m_originY + y)][layer];
	block.blockID = id;
	block.replace = replace;
}

void FormationElement::setBlockEntityAt(const int x, const int y, const WorldLayer layer, BlockEntity *blockEntity)
{
	// Expand bounding box
	if(x < 0) m_x1 = min(m_x1, m_originX + x);
	else m_x2 = max(m_x2, m_originX + x);
	if(y < 0) m_y1 = min(m_y1, m_originY + y);
	else m_y2 = max(m_y2, m_originY + y);

	// Set block at position
	m_blocks[CHUNK_KEY(m_originX + x, m_originY + y)][layer].blockEntity = blockEntity;
}
