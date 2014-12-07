#ifndef TERRAIN_GEN_H
#define TERRAIN_GEN_H

#include <x2d/x2d.h>
#include "constants.h"
#include "structure.h"

enum BlockID;
enum TerrainLayer;

extern float step(float edge, float x);

#include "terrain/simplex.h"

class TerrainGen
{
	friend class Terrain;
public:

	static BlockID getBlockAt(const int x, const int y, const TerrainLayer layer);
	static BlockID getGroundAt(const int x, const int y, const TerrainLayer layer);
	static int getGroundHeight(const int x);
	static void loadStructures(const int x, const int y);

private:
	static uint s_seed;
	static Simplex2D s_noise;
	static XRandom s_random;
	static map<uint, map<uint, BlockID*>> s_structureTiles;
};

#endif // TERRAIN_GEN_H