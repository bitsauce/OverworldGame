#include "terraingen.h"
#include "game.h"
#include "grassland.h"

float step(float edge, float x)
{
	return x < edge ? 0.0f : 1.0f;
}

uint TerrainGen::s_seed;
Simplex2D TerrainGen::s_noise;
xd::Random TerrainGen::s_random;
map<int64_t, TerrainGen::BlockUnion> TerrainGen::s_structureMap;
unordered_set<uint> TerrainGen::s_loadedSuperChunks;

#define CHUNK_KEY(X, Y) (((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000))
#define BLOCK_KEY(X, Y) ((int64_t(X) & 0x00000000FFFFFFFF) | ((int64_t(Y) << 32) & 0xFFFFFFFF00000000))

BlockID TerrainGen::getBlockAt(const int x, const int y, const TerrainLayer layer)
{
	BlockID block = getGroundAt(x, y, layer);
		
	// TODO: Move this check somewhere else
	/*int superChunkX = xd::math::floor(x/SUPER_CHUNK_BLOCKSF), superChunkY = xd::math::floor(y/SUPER_CHUNK_BLOCKSF);
	uint key = CHUNK_KEY(superChunkX, superChunkY);
	if(s_loadedSuperChunks.find(key) == s_loadedSuperChunks.end())
	{
		loadStructures(superChunkX, superChunkY);
		s_loadedSuperChunks.insert(key);
	}
		
	// Apply structures
	int64_t blockKey = BLOCK_KEY(x, y);
	if(s_structureMap.find(blockKey) != s_structureMap.end())
	{
		BlockUnion &blocks = s_structureMap[blockKey];
		switch(layer)
		{
		case TERRAIN_LAYER_BACK: if(blocks.back > 0) block = blocks.back; break;
		case TERRAIN_LAYER_MIDDLE: if(blocks.middle > 0) block = blocks.middle; break;
		case TERRAIN_LAYER_FRONT: if(blocks.front > 0) block = blocks.front; break;
		}
	}*/
	return block;
}
	
BlockID TerrainGen::getGroundAt(const int x, const int y, const TerrainLayer layer)
{
	switch(layer)
	{
		case TERRAIN_LAYER_MIDDLE:
		{
			float h = s_noise.valueAt(x*0.1f, y + 710239) * 7;

			// Ground
			if((math::clamp((32 - y)/32.0f, 0.0f, 1.0f) + (s_noise.valueAt(x, y) * 0.5f + 0.5f)) * step(0, y + h) > 0.5f)
			{
				return BLOCK_GRASS;
			}
		}
		break;
			
		case TERRAIN_LAYER_BACK:
		{
			if(y > 20)
			{
				return BLOCK_STONE;
			}
		}
		break;
	}
	return BLOCK_EMPTY;
}

void TerrainGen::loadStructures(const int superChunkX, const int superChunkY)
{
	xd::LOG("Placing structures in super chunk [%i, %i]", superChunkX, superChunkY);
	//vector<Structure*> structures;
	for(int x = 0; x < SUPER_CHUNK_BLOCKS; ++x)
	{
		break;
		int tileX = SUPER_CHUNK_BLOCKS * superChunkX + x;
		if(s_random.getDouble(tileX + s_seed) < 0.005/*TREE_CHANCE*/)
		{
			//Tree *tree = new Tree;
			//tree->x = tileX;
			//tree->y = getGroundHeight(tileX);
			//structures.push_back(tree);

			int tileY = getGroundHeight(tileX);
			int length = 8 + s_random.getDouble(tileX + s_seed + 8572) * 32;

			// Stem and lower branches
			for(int h = int(length * 0.5f); h >= 0; --h)
			{
				/*if(h > length * 0.2f && s_random.getDouble(tileX + s_seed + h) < 0.1)
				{
					// Create lower branches
					int w = s_random.getDouble(tileX + s_seed + h + 1392) * h; // Branch length
					bool dir = s_random.getDouble(tileX + s_seed + h + 844135) < 0.5; // Left or right?
					int x1 = 1, y1 = h; // Branch offsets
					int w1 = w*0.5f; // Half length
					while(true)
					{
						// Set branch block
						s_structureMap[BLOCK_KEY(tileX + (dir ? x1 : -x1), tileY - y1)].back = BLOCK_WOOD;
						
						// Have we reached the half of the remainding branch?
						if(w1-- <= 0)
						{
							// Increase branch height
							y1++;

							// Calculate half of remaining branch
							w1 = (w-x1)*0.5f;
							if(w1 < 1) break; // It'll look prettier this way

							// Set the block here 
							s_structureMap[BLOCK_KEY(tileX + (dir ? x1 : -x1), tileY - y1)].back = BLOCK_WOOD;
						}

						x1++;
					}
				}*/

				s_structureMap[BLOCK_KEY(tileX, tileY - h)].back = BLOCK_WOOD;
			}

			// HOW IT (SHOULD) WORK(S):
			// 1) From the end of the branch, choose a random number of branches to create.
			// 2) For each branch, pick a random angle between [45, 135] (relative to the angle of the branch)
			//    and a random branch length.
			// 3) Use line rasterization to create the branch segment.
			// 4) Repeat until a branch with a total length of length/2 is created.

			// Upper brances
			vector<Vector4> branchPoints;
			branchPoints.push_back(Vector4(0.0f, length * 0.5f, PI * 0.5f, length));
			while(!branchPoints.empty())
			{
				Vector4 branch = branchPoints.back();
				branchPoints.pop_back();

				int r = 5;
				for(int j = -r; j < r; ++j)
				{
					for(int i = -r; i < r; ++i)
					{
						if(sqrt(j*j+i*i) < r)
						{
							s_structureMap[BLOCK_KEY(tileX + branch.x + i, tileY - branch.y + j)].front = BLOCK_LEAF;
						}
					}
				}

				int branchCount = 2;// + (s_random.getInt(tileX + s_seed + 88372) % 3);
				for(int i = 0; i < branchCount; ++i)
				{
					//float segmentAngle = branch.z - (s_random.getDouble(tileX + s_seed + (branch.x + branch.y + branch.w) * 3628 + i * 8731) * PI * 0.5f) - PI * 0.25f; // From [45, 135]
					float segmentAngle = branch.z + PI * 0.25f * (i == 0 ? 1 : -1); // From [45, 135]
					float segmentLength = branch.w * 0.15f;
					if(segmentLength > 3.0f)
					{
						RayCast ray;
						ray.test(Vector2(branch.x, branch.y), Vector2(branch.x, branch.y) + Vector2(cos(segmentAngle), sin(segmentAngle)) * segmentLength);
					
						vector<Vector2i> points = ray.getPoints();
						for(uint j = 0; j < points.size(); ++j)
						{
							s_structureMap[BLOCK_KEY(tileX + points[j].x, tileY - points[j].y)].back = BLOCK_WOOD;
						}

						branchPoints.push_back(Vector4(points.back().x, points.back().y, segmentAngle, branch.w - segmentLength));
					}
				}
			}
		}
	}
		
	// Place structures
	/*for(uint i = 0; i < structures.size(); ++i)
	{
		Structure *structure = structures[i];
		for(int y = 0; y < structure->height; ++y)
		{
			for(int x = 0; x < structure->width; ++x)
			{
				BlockUnion blocks;
				for(int i = 0; i < TERRAIN_LAYER_COUNT; ++i)
				{
					BlockID structTile = structure->getBlockAt(x, y, TerrainLayer(i));
					if(structTile > 0)
					{
						switch(i)
						{
						case TERRAIN_LAYER_BACK: blocks.back = structTile; break;
						case TERRAIN_LAYER_MIDDLE: blocks.middle = structTile; break;
						case TERRAIN_LAYER_FRONT: blocks.front = structTile; break;
						}
					}
				}
				if(blocks.back == BLOCK_EMPTY && blocks.front == BLOCK_EMPTY && blocks.middle == BLOCK_EMPTY) continue;
				int structX = structure->x + x - structure->originX, structY = structure->y + y - structure->originY;
				s_structureMap[BLOCK_KEY(structX, structY)] = blocks;
			}
		}
	}*/

	//for(uint i = 0; i < structures.size(); ++i) delete structures[i];
}
	
int TerrainGen::getGroundHeight(const int x)
{
	int y = 0;
	while(getGroundAt(x, y++, TERRAIN_LAYER_MIDDLE) == BLOCK_EMPTY);
	return y-1;
}
	
bool isFlatStretch(int start, int size)
{
	/*int height = getGroundHeight(start);
	for(int x = start+1; x < start+size; x++)
	{
		if(getGroundHeight(x) != height)
			return false;
	}*/
	return true;
}