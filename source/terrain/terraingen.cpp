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

Simplex2D TerrainGen::s_noise;

BlockID TerrainGen::getBlockAt(const int x, const int y, const TerrainLayer layer)
{
	switch(layer)
	{
		case TERRAIN_LAYER_SCENE:
		{
			float h = s_noise.valueAt(x*0.1f, y + 710239) * 7;

			// Ground
			if((XMath::clamp((100 - y)/100.0f, 0.0f, 1.0f) + (s_noise.valueAt(x, y) * 0.5f + 0.5f)) * step(0, y + h) > 0.5f)
			{
				return BLOCK_SCENE_GRASS;
			}
		}
		break;
			
		case TERRAIN_LAYER_BACKGROUND:
		{
			//if(y > 20)
			//	return STONE_WALL;
		}
		break;
	}
	return BLOCK_EMPTY;
}