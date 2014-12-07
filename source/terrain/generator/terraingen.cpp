#include "terraingen.h"
#include "constants.h"
#include "grassland.h"

float step(float edge, float x)
{
	return x < edge ? 0.0f : 1.0f;
}

Simplex2D TerrainGen::s_noise;

BlockID TerrainGen::getBlockAt(const int x, const int y, const TerrainLayer layer)
{
	switch(layer)
	{
		case TERRAIN_LAYER_SCENE:
		{
			/*float h = s_noise.valueAt(x*0.1f, y + 710239) * 7;

			// Ground
			if((XMath::clamp((100 - y)/100.0f, 0.0f, 1.0f) + (s_noise.valueAt(x, y) * 0.5f + 0.5f)) * step(0, y + h) > 0.5f)
			{
				return BLOCK_GRASS;
			}*/

			if(sin(x*0.05f)*10 + 30 < y)
			{
				if(sin(x*0.05f)*10 + 40 < y && s_noise.valueAt(x, y) > 0.0f)
					return BLOCK_STONE;
				return BLOCK_GRASS;
			}
			return BLOCK_EMPTY;
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