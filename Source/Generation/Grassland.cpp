#include "grassland.h"
#include "constants.h"

/*Tree::Tree()
{
}

BlockID Tree::getBlockAt(const int x, const int y, const WorldLayer layer)
{
	switch(layer)
	{
		case WORLD_LAYER_BACK:
		{
			if(x == 5 && y >= 5)
			{
				return BLOCK_WOOD_OAK;
			}
		}
			
		case WORLD_LAYER_FRONT:
		{
			if(sqrt(pow(y-5, 2)+pow(x-5, 2)) < 5.0f || sqrt(pow(y-10, 2)+pow(x-5, 2)) < 15.0f)
			{
				return BLOCK_LEAF;
			}
		}
	}
	return BlockID(-1);
}*/