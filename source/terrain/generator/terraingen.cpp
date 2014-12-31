#include "terraingen.h"
#include "constants.h"
#include "grassland.h"

float step(float edge, float x)
{
	return x < edge ? 0.0f : 1.0f;
}

uint TerrainGen::s_seed;
Simplex2D TerrainGen::s_noise;
XRandom TerrainGen::s_random;
map<int64_t, TerrainGen::BlockUnion> TerrainGen::s_structureMap;
unordered_set<uint> TerrainGen::s_loadedSuperChunks;

#define CHUNK_KEY(X, Y) (((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000))
#define BLOCK_KEY(X, Y) ((int64_t(X) & 0x00000000FFFFFFFF) | ((int64_t(Y) << 32) & 0xFFFFFFFF00000000))

BlockID TerrainGen::getBlockAt(const int x, const int y, const TerrainLayer layer)
{
	BlockID block = getGroundAt(x, y, layer);
		
	// TODO: Move this check somewhere else
	int superChunkX = XMath::floor(x/SUPER_CHUNK_BLOCKSF), superChunkY = XMath::floor(y/SUPER_CHUNK_BLOCKSF);
	uint key = CHUNK_KEY(superChunkX, superChunkY);
	if(s_loadedSuperChunks.find(key) == s_loadedSuperChunks.end())
	{
		loadStructures(superChunkX, superChunkY);
		s_loadedSuperChunks.insert(key);
	}
		
	// Apply structures
	int64_t blockKey = BLOCK_KEY(x, y);
	if(s_structureMap.find(blockKey) != s_structureMap.end())
	{
		BlockUnion &blocks = s_structureMap[blockKey];
		switch(layer)
		{
		case TERRAIN_LAYER_BACK: if(blocks.back > 0) block = blocks.back; break;
		case TERRAIN_LAYER_MIDDLE: if(blocks.middle > 0) block = blocks.middle; break;
		case TERRAIN_LAYER_FRONT: if(blocks.front > 0) block = blocks.front; break;
		}
	}
	return block;
}
	
BlockID TerrainGen::getGroundAt(const int x, const int y, const TerrainLayer layer)
{
	switch(layer)
	{
		case TERRAIN_LAYER_MIDDLE:
		{
			float h = s_noise.valueAt(x*0.1f, y + 710239) * 7;

			// Ground
			if((XMath::clamp((100 - y)/100.0f, 0.0f, 1.0f) + (s_noise.valueAt(x, y) * 0.5f + 0.5f)) * step(0, y + h) > 0.5f)
			{
				return BLOCK_GRASS;
			}
		}
		break;
			
		case TERRAIN_LAYER_BACK:
		{
			if(y > 20)
			{
				return BLOCK_STONE;
			}
		}
		break;
	}
	return BLOCK_EMPTY;
}
	
void TerrainGen::loadStructures(const int superChunkX, const int superChunkY)
{
	LOG("Placing structures in super chunk [%i, %i]", superChunkX, superChunkY);
	//vector<Structure*> structures;
	for(int x = 0; x < SUPER_CHUNK_BLOCKS; ++x)
	{
		int tileX = SUPER_CHUNK_BLOCKS * superChunkX + x;
		if(s_random.getDouble(tileX + s_seed) < 0.05/*TREE_CHANCE*/)
		{
			//Tree *tree = new Tree;
			//tree->x = tileX;
			//tree->y = getGroundHeight(tileX);
			//structures.push_back(tree);

			int tileY = getGroundHeight(tileX);
			int height = 8 + s_random.getDouble(tileX + s_seed + 8572) * 8;
			for(int h = height; h >= 0; --h)
			{
				/*if(h > height * 0.25 && s_random.getDouble(tileY - h + s_seed) < 0.2*(height-h)/float(height))
				{
					for(int w = -10; w < 10; w++)
						s_structureMap[BLOCK_KEY(tileX + w, tileY - h)].back = BLOCK_WOOD;
				}*/

				s_structureMap[BLOCK_KEY(tileX, tileY - h)].back = BLOCK_WOOD;
			}

			int r = 8 * height/16.0f;
			for(int j = -r; j < r; ++j)
			{
				for(int i = -r; i < r; ++i)
				{
					if(sqrt(j*j+i*i) <= r * 0.5f)
					{
						s_structureMap[BLOCK_KEY(tileX + i, tileY + j - height)].front = BLOCK_LEAF;
					}
				}
			}

			r = 12 * height/16.0f;
			for(int j = -r; j < r; ++j)
			{
				for(int i = -r; i < r; ++i)
				{
					if(sqrt(j*j+i*i) <= r * 0.5f)
					{
						s_structureMap[BLOCK_KEY(tileX + i, tileY + j - height + int(6.0f*height/20.0f))].front = BLOCK_LEAF;
					}
				}
			}
		}
	}
		
	// Place structures
	/*for(uint i = 0; i < structures.size(); ++i)
	{
		Structure *structure = structures[i];
		for(int y = 0; y < structure->height; ++y)
		{
			for(int x = 0; x < structure->width; ++x)
			{
				BlockUnion blocks;
				for(int i = 0; i < TERRAIN_LAYER_COUNT; ++i)
				{
					BlockID structTile = structure->getBlockAt(x, y, TerrainLayer(i));
					if(structTile > 0)
					{
						switch(i)
						{
						case TERRAIN_LAYER_BACK: blocks.back = structTile; break;
						case TERRAIN_LAYER_MIDDLE: blocks.middle = structTile; break;
						case TERRAIN_LAYER_FRONT: blocks.front = structTile; break;
						}
					}
				}
				if(blocks.back == BLOCK_EMPTY && blocks.front == BLOCK_EMPTY && blocks.middle == BLOCK_EMPTY) continue;
				int structX = structure->x + x - structure->originX, structY = structure->y + y - structure->originY;
				s_structureMap[BLOCK_KEY(structX, structY)] = blocks;
			}
		}
	}*/

	//for(uint i = 0; i < structures.size(); ++i) delete structures[i];
}
	
int TerrainGen::getGroundHeight(const int x)
{
	int y = 0;
	while(getGroundAt(x, y++, TERRAIN_LAYER_MIDDLE) == BLOCK_EMPTY);
	return y-1;
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