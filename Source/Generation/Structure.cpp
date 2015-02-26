#include "structure.h"
#include "constants.h"

Structure::Structure() :
	x(0),
	y(0),
	width(0),
	height(0),
	originX(0),
	originY(0)
{
}

BlockID Structure::getBlockAt(const int /*x*/, const int /*y*/, const TerrainLayer /*layer*/)
{
	return BLOCK_EMPTY;
}