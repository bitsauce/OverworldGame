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
public:

	uint seed;

	static BlockID getBlockAt(const int x, const int y, const TerrainLayer);

private:
	static Simplex2D s_noise;
};

#endif // TERRAIN_GEN_H