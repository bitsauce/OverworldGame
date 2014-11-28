#include "terraingen.h"
#include "constants.h"

float step(float edge, float x)
{
	return x < edge ? 0.0f : 1.0f;
}

BlockID Tree::getBlockAt(const int x, const int y, TerrainLayer layer)
{
	switch(layer)
	{
		case TERRAIN_LAYER_BACKGROUND:
		{
			if(x == 5 && y >= 5)
			{
				return BLOCK_BACKGROUND_WOOD;
			}
		}
			
		case TERRAIN_LAYER_FOREGROUND:
		{
			if(sqrt(pow(y-5, 2)+pow(x-5, 2)) < 5.0f)
			{
				return BLOCK_FOREGROUND_LEAF;
			}
		}
	}
	return BlockID(-1);
}

BlockID TerrainGen::getBlockAt(const int x, const int y, const TerrainLayer layer)
{
	if(layer == TERRAIN_LAYER_SCENE)
		return sin(x*0.1f)*10.0f < y ? BLOCK_SCENE_GRASS : BLOCK_EMPTY;
	return BLOCK_EMPTY;
}