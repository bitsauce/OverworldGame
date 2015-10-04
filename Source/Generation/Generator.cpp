#include "Generator.h"
#include "Grassland.h"
#include "Game/RayCast.h"

#include "Structure.h"
#include "Structures/OakTree.h"

float step(float edge, float x)
{
	return x < edge ? 0.0f : 1.0f;
}

#define CHUNK_KEY(X, Y) (((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000))

WorldGenerator::WorldGenerator(const uint seed) :
	m_seed(seed)
{
	m_random.setSeed(seed);
}

void WorldGenerator::getChunkBlocks(const int chunkX, const int chunkY, BlockID *blocks)
{
	// Load structures
	loadStructures(chunkX, chunkY);

	// Load structures?
	// TODO: Not sure this should be here, but it cant be in WorldGenerator::getChunkBlocks either because
	// the chunk has to be loaded when we know that all the structures that will affect this chunk are generated.
	BlockID *structureBlocks = m_chunkStructures[CHUNK_KEY(chunkX, chunkY)];

	// Load blocks
	const int tileX = chunkX * CHUNK_BLOCKS;
	const int tileY = chunkY * CHUNK_BLOCKS;
	for(uint y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(uint x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(uint z = 0; z < TERRAIN_LAYER_COUNT; ++z)
			{
				BlockID structureBlock;
				if((structureBlock = structureBlocks[BLOCK_INDEX(x, y, z)]) == BLOCK_EMPTY)
				{
					blocks[BLOCK_INDEX(x, y, z)] = getGroundAt(tileX + x, tileY + y, (TerrainLayer) z);
				}
				else
				{
					blocks[BLOCK_INDEX(x, y, z)] = structureBlock;
				}
			}
		}
	}
}
	
BlockID WorldGenerator::getGroundAt(const int x, const int y, const TerrainLayer layer)
{
	switch(layer)
	{
		case TERRAIN_LAYER_MIDDLE:
		{
			float h = m_noise.valueAt(x * 0.1f, y + 710239) * 7;

			// Ground
			if((math::clamp((32 - y) / 32.0f, 0.0f, 1.0f) + (m_noise.valueAt(x, y) * 0.5f + 0.5f)) * step(0, y + h) > 0.5f)
			{
				float f = m_noise.valueAt(x + 2989, y + 7238) * 0.5f + 0.5f;

				f *= (math::clamp(y / 1000.0f, 0.0f, 1.0f));

				return (f < 0.1f) ? BLOCK_GRASS : BLOCK_STONE;
			}
		}
		break;
			
		case TERRAIN_LAYER_BACK:
		{
			if(y > 20)
			{
				return BLOCK_DIRT_BACK;
			}
		}
		break;
	}
	return BLOCK_EMPTY;
}

void WorldGenerator::loadStructures(const int chunkX, const int chunkY)
{
	// Create list
	list<Structure*> structures;
	if(m_chunkStructures.find(CHUNK_KEY(chunkX, chunkY)) == m_chunkStructures.end())
	{
		BlockID *blocks = new BlockID[CHUNK_BLOCKS * CHUNK_BLOCKS * TERRAIN_LAYER_COUNT];
		for(int i = 0; i < CHUNK_BLOCKS * CHUNK_BLOCKS * TERRAIN_LAYER_COUNT; ++i)
		{
			blocks[i] = BLOCK_EMPTY;
		}
		m_chunkStructures[CHUNK_KEY(chunkX, chunkY)] = blocks;
	}

	// Load structures
	for(int x = 0; x < CHUNK_BLOCKS; ++x)
	{
		int tileX = CHUNK_BLOCKS * chunkX + x;
		if(m_random.getDouble(tileX + m_seed) < 0.025/*TREE_CHANCE*/)
		{
			structures.push_back(new OakTree(tileX, getGroundHeight(tileX)));
		}
	}

	// Place structures
	for(Structure *structure : structures)
	{
		structure->place(this);
		delete structure;
	}
}

void WorldGenerator::setBlockAt(const int x, const int y, const TerrainLayer z, const BlockID block)
{
	int chunkX = (int) floor(x / CHUNK_BLOCKSF),
		chunkY = (int) floor(y / CHUNK_BLOCKSF);
	if(m_chunkStructures.find(CHUNK_KEY(chunkX, chunkY)) == m_chunkStructures.end())
	{
		BlockID *blocks = new BlockID[CHUNK_BLOCKS * CHUNK_BLOCKS * TERRAIN_LAYER_COUNT];
		for(int i = 0; i < CHUNK_BLOCKS * CHUNK_BLOCKS * TERRAIN_LAYER_COUNT; ++i)
		{
			blocks[i] = BLOCK_EMPTY;
		}
		m_chunkStructures[CHUNK_KEY(chunkX, chunkY)] = blocks;
	}
	m_chunkStructures[CHUNK_KEY(chunkX, chunkY)][BLOCK_INDEX(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), z)] = block;
}

int WorldGenerator::getGroundHeight(const int x)
{
	int y = 0;
	if(getGroundAt(x, y, TERRAIN_LAYER_MIDDLE) == BLOCK_EMPTY)
	{
		while(getGroundAt(x, ++y, TERRAIN_LAYER_MIDDLE) == BLOCK_EMPTY); y--;
	}
	else
	{
		while(getGroundAt(x, --y, TERRAIN_LAYER_MIDDLE) != BLOCK_EMPTY);
	}
	return y;
}
	
bool isFlatStretch(int start, int size)
{
	/*int height = getGroundHeight(start);
	for(int x = start+1; x < start+size; x++)
	{
		if(getGroundHeight(x) != height)
			return false;
	}*/
	return true;
}