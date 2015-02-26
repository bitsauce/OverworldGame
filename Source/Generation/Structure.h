#ifndef STRUCTURE_H
#define STRUCTURE_H

enum BlockID;
enum TerrainLayer;

class Structure
{
public:
	Structure();

	virtual BlockID getBlockAt(const int x, const int y, const TerrainLayer layer);
	
	int x, y;
	int width, height;
	int originX, originY;
};

#endif // STRUCTURE_H