#ifndef GRASSLAND_H
#define GRASSLAND_H

#include <x2d/x2d.h>
#include "structure.h"

class Tree : public Structure
{
public:
	Tree();
	
	BlockID getBlockAt(const int x, const int y, const TerrainLayer layer);
};

#endif // GRASSLAND_H