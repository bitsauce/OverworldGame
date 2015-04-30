#include "Generator.h"
#include "Grassland.h"
#include "Game/RayCast.h"

float step(float edge, float x)
{
	return x < edge ? 0.0f : 1.0f;
}

#define CHUNK_KEY(X, Y) (((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000))

void WorldGenerator::getChunkBlocks(const int chunkX, const int chunkY, BlockID *blocks)
{
	// Load super chunk if not loaded
	int superChunkX = (int) floor(chunkX / SUPER_CHUNK_CHUNKSF), superChunkY = (int) floor(chunkY / SUPER_CHUNK_CHUNKSF);
	uint key = CHUNK_KEY(superChunkX, superChunkY);
	if(m_loadedSuperChunks.find(key) == m_loadedSuperChunks.end())
	{
		loadStructures(superChunkX, superChunkY);
		m_loadedSuperChunks.insert(key);
	}

	// Load blocks
	const int tileX = chunkX * CHUNK_BLOCKS;
	const int tileY = chunkY * CHUNK_BLOCKS;
	for(uint y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(uint x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(uint z = 0; z < TERRAIN_LAYER_COUNT; ++z)
			{
				tuple<int, int, int> key = make_tuple(tileX + x, tileY + y, z);
				if(m_structureMap.find(key) != m_structureMap.end() && m_structureMap[key] > BLOCK_EMPTY)
				{
					blocks[BLOCK_INDEX(x, y, z)] = m_structureMap[key];
				}
				else
				{
					blocks[BLOCK_INDEX(x, y, z)] = getGroundAt(tileX + x, tileY + y, (TerrainLayer)z);
				}
			}
		}
	}
}
	
BlockID WorldGenerator::getGroundAt(const int x, const int y, const TerrainLayer layer)
{
	switch(layer)
	{
		case TERRAIN_LAYER_MIDDLE:
		{
			float h = m_noise.valueAt(x*0.1f, y + 710239) * 7;

			// Ground
			if((math::clamp((32 - y)/32.0f, 0.0f, 1.0f) + (m_noise.valueAt(x, y) * 0.5f + 0.5f)) * step(0, y + h) > 0.5f)
			{
				return m_noise.valueAt(x + 2989, y + 7238) < 0.0 ? BLOCK_GRASS : BLOCK_STONE;//y <= 100 ? BLOCK_GRASS : BLOCK_STONE;
			}
		}
		break;
			
		case TERRAIN_LAYER_BACK:
		{
			if(y > 20)
			{
				return BLOCK_DIRT_BACK;
			}
		}
		break;
	}
	return BLOCK_EMPTY;
}

void WorldGenerator::loadStructures(const int superChunkX, const int superChunkY)
{
	LOG("Placing structures in super chunk [%i, %i]", superChunkX, superChunkY);
	
	Random s_random;
	uint s_seed = 0;
	//vector<Structure*> structures;

	// Load structures
	for(int x = 0; x < SUPER_CHUNK_BLOCKS; ++x)
	{
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

				m_structureMap[make_tuple(tileX, tileY - h, TERRAIN_LAYER_BACK)] = BLOCK_WOOD;
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
							m_structureMap[make_tuple(tileX + branch.x + i, tileY - branch.y + j, TERRAIN_LAYER_FRONT)] = BLOCK_LEAF;
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
							m_structureMap[make_tuple(tileX + points[j].x, tileY - points[j].y, TERRAIN_LAYER_BACK)] = BLOCK_WOOD;
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

int WorldGenerator::getGroundHeight(const int x)
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