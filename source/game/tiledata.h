#ifndef TILE_DATA_H
#define TILE_DATA_H

#include <x2d/x2d.h>

enum BlockID;
class b2Fixture;

class Tile
{
public:

	void setupFixture(b2Fixture *fixture)
	{}

	vector<XVertex> getVertices(const int x, const int y, const uint state);
	vector<uint> getIndices();

private:

};

class TileData
{
public:

	static Tile &get(const BlockID id);

private:

};

#endif // TILE_DATA_H