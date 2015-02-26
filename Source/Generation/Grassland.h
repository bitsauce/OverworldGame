#ifndef GRASSLAND_H
#define GRASSLAND_H

#include "Config.h"
#include "Structure.h"

class Tree : public Structure
{
public:
	Tree();
	
	BlockID getBlockAt(const int x, const int y, const TerrainLayer layer);
};

#endif // GRASSLAND_H