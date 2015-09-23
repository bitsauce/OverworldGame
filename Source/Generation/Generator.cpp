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

void WorldGenerator::getChunkBlocks(const int chunkX, const int chunkY, BlockID *blocks)
{
	// Load super chunk if not loaded
	/*int superChunkX = (int) floor(chunkX / SUPER_CHUNK_CHUNKSF), superChunkY = (int) floor(chunkY / SUPER_CHUNK_CHUNKSF);
	uint key = CHUNK_KEY(superChunkX, superChunkY);
	if(m_loadedSuperChunks.find(key) == m_loadedSuperChunks.end())
	{
		loadStructures(superChunkX, superChunkY);
		m_loadedSuperChunks.insert(key);
	}*/

	// Load blocks
	const int tileX = chunkX * CHUNK_BLOCKS;
	const int tileY = chunkY * CHUNK_BLOCKS;
	for(uint y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(uint x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(uint z = 0; z < TERRAIN_LAYER_COUNT; ++z)
			{
				/*tuple<int, int, int> key = make_tuple(tileX + x, tileY + y, z);
				if(m_structureMap.find(key) != m_structureMap.end() && m_structureMap[key] > BLOCK_EMPTY)
				{
					blocks[BLOCK_INDEX(x, y, z)] = m_structureMap[key];
				}
				else*/
				{
					blocks[BLOCK_INDEX(x, y, z)] = getGroundAt(tileX + x, tileY + y, (TerrainLayer)z);
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

void WorldGenerator::loadStructures(const int superChunkX, const int superChunkY)
{
	LOG("Placing structures in super chunk [%i, %i]", superChunkX, superChunkY);
	
	Random s_random;
	uint s_seed = 0;
	list<Structure*> structures;

	// Load structures
	for(int x = 0; x < SUPER_CHUNK_BLOCKS; ++x)
	{
		int tileX = SUPER_CHUNK_BLOCKS * superChunkX + x;
		if(s_random.getDouble(tileX + s_seed) < 0.005/*TREE_CHANCE*/)
		{
			structures.push_back(new OakTree(tileX, getGroundHeight(tileX)));
		}
	}

	// Place structures
	StructurePlacer structPlacer(&m_structureMap);
	for(Structure *structure : structures)
	{
		structure->place(this, &structPlacer);
		delete structure;
	}
}

void StructurePlacer::setBlockAt(const int x, const int y, const TerrainLayer z, const BlockID block)
{
	(*m_structureMap)[make_tuple(x, y, z)] = block;
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