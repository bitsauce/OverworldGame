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
map<uint, map<uint, BlockID*>> TerrainGen::s_structureTiles;

#define CHUNK_KEY(X, Y) (((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000))
#define BLOCK_KEY(X, Y) (((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000))

BlockID TerrainGen::getBlockAt(const int x, const int y, const TerrainLayer layer)
{
	BlockID block = getGroundAt(x, y, layer);
		
	// TODO: Move this check somewhere else
	int superChunkX = XMath::floor(x/SUPER_CHUNK_BLOCKSF), superChunkY = XMath::floor(y/SUPER_CHUNK_BLOCKSF);
	uint key = CHUNK_KEY(superChunkX, superChunkY);
	if(s_structureTiles.find(key) == s_structureTiles.end())
	{
		s_structureTiles[key] = map<uint, BlockID*>();
		loadStructures(superChunkX, superChunkY);
	}
		
	// Apply structures
	ulong blockKey = BLOCK_KEY(x, y);
	if(s_structureTiles[key].find(blockKey) != s_structureTiles[key].end())
	{
		block = s_structureTiles[key][blockKey][layer];
	}
	return block;
}
	
BlockID TerrainGen::getGroundAt(const int x, const int y, const TerrainLayer layer)
{
	switch(layer)
	{
		case TERRAIN_LAYER_SCENE:
		{
			float h = s_noise.valueAt(x*0.1f, y + 710239) * 7;

			// Ground
			if((XMath::clamp((100 - y)/100.0f, 0.0f, 1.0f) + (s_noise.valueAt(x, y) * 0.5f + 0.5f)) * step(0, y + h) > 0.5f)
			{
				return BLOCK_GRASS;
			}
		}
		break;
			
		case TERRAIN_LAYER_BACKGROUND:
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
	vector<Structure*> structures;
	for(int x = 0; x < SUPER_CHUNK_BLOCKS; ++x)
	{
		int tileX = SUPER_CHUNK_BLOCKS * superChunkX + x;
		if(s_random.getDouble(tileX + s_seed) < 0.05/*TREE_CHANCE*/)
		{
			Tree *tree = new Tree;
			tree->x = tileX;
			tree->y = getGroundHeight(tileX);
			structures.push_back(tree);
		}
	}
		
	// Place structures
	for(uint i = 0; i < structures.size(); ++i)
	{
		Structure *structure = structures[i];
		for(int y = 0; y < structure->height; ++y)
		{
			for(int x = 0; x < structure->width; ++x)
			{
				int structX = structure->x + x - structure->originX, structY = structure->y + y - structure->originY;
				for(int i = 0; i < TERRAIN_LAYER_COUNT; ++i)
				{
					BlockID structTile = structure->getBlockAt(x, y, TerrainLayer(i));
					if(structTile > 0) {
						uint blockKey = BLOCK_KEY(structX, structY, i);
						if(s_structureTiles[CHUNK_KEY(superChunkX, superChunkY)].find(blockKey) == s_structureTiles[CHUNK_KEY(superChunkX, superChunkY)].end())
						{
							BlockID *blocks = s_structureTiles[CHUNK_KEY(superChunkX, superChunkY)][blockKey] = new BlockID[TERRAIN_LAYER_COUNT];
							blocks[0] = blocks[1] = blocks[2] = BLOCK_EMPTY;
						}
						s_structureTiles[CHUNK_KEY(superChunkX, superChunkY)][blockKey][i] = structTile;
					}
				}
			}
		}
	}

	for(uint i = 0; i < structures.size(); ++i) delete structures[i];
}
	
int TerrainGen::getGroundHeight(const int x)
{
	int y = 0;
	while(getGroundAt(x, y++, TERRAIN_LAYER_SCENE) == BLOCK_EMPTY);
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