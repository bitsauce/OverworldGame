#ifndef TERRAIN_GEN_H
#define TERRAIN_GEN_H

#include <x2d/x2d.h>
#include "constants.h"
#include "structure.h"

enum BlockID;
enum TerrainLayer;

extern float step(float edge, float x);

#include "Noise/Simplex.h"
#include <unordered_set>

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

	struct BlockUnion
	{
		BlockUnion() : back(BLOCK_EMPTY), middle(BLOCK_EMPTY), front(BLOCK_EMPTY) {}
		BlockID back;
		BlockID middle;
		BlockID front;
	};

	static map<int64_t, BlockUnion> s_structureMap;
	static unordered_set<uint> s_loadedSuperChunks;
};

#endif // TERRAIN_GEN_H