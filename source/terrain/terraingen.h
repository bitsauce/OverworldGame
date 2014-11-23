#ifndef TERRAIN_GEN_H
#define TERRAIN_GEN_H

#include <x2d/x2d.h>
#include "constants.h"

enum BlockID;
enum TerrainLayer;

extern float step(float edge, float x);

class Structure
{
public:
	int x, y;
	int width, height;
	int originX, originY;

	Structure() :
		x(0),
		y(0),
		width(0),
		height(0),
		originX(0),
		originY(0)
	{
	}
	
	BlockID getTileAt(const int x, const int y, TerrainLayer layer)
	{
		return EMPTY_TILE;
	}
};

class Tree : public Structure
{
public:
	Tree()
	{
		width = 10;
		height = 15;
		originX = 5;
		originY = 15;
	}
	
	BlockID getTileAt(const int x, const int y, TerrainLayer layer);
};

class TerrainGen
{
public:
	uint seed;

	static BlockID getTileAt(const int x, const int y, const TerrainLayer);
};

#endif // TERRAIN_GEN_H