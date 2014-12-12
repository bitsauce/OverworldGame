#include "grassland.h"
#include "constants.h"

Tree::Tree()
{
	width = 10;
	height = 15;
	originX = 5;
	originY = 15;
}

BlockID Tree::getBlockAt(const int x, const int y, TerrainLayer layer)
{
	switch(layer)
	{
		case TERRAIN_LAYER_BACKGROUND:
		{
			if(x == 5 && y >= 5)
			{
				return BLOCK_WOOD;
			}
		}
			
		case TERRAIN_LAYER_FOREGROUND:
		{
			if(sqrt(pow(y-5, 2)+pow(x-5, 2)) < 5.0f || sqrt(pow(y-10, 2)+pow(x-5, 2)) < 15.0f)
			{
				return BLOCK_LEAF;
			}
		}
	}
	return BlockID(-1);
}