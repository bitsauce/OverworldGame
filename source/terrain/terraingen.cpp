#include "terraingen.h"
#include "constants.h"

float step(float edge, float x)
{
	return x < edge ? 0.0f : 1.0f;
}

BlockID Tree::getTileAt(const int x, const int y, TerrainLayer layer)
{
	switch(layer)
	{
		case TERRAIN_LAYER_BACKGROUND:
		{
			if(x == 5 && y >= 5)
			{
				return WOOD_TILE;
			}
		}
			
		case TERRAIN_LAYER_FOREGROUND:
		{
			if(sqrt(pow(y-5, 2)+pow(x-5, 2)) < 5.0f)
			{
				return LEAF_TILE;
			}
		}
	}
	return NULL_TILE;
}

BlockID TerrainGen::getTileAt(const int x, const int y, const TerrainLayer)
{
	return EMPTY_TILE;
}