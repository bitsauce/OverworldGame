#ifndef TERRAIN_H
#define TERRAIN_H

#include "Config.h"

#include "World/ChunkManager.h"
#include "World/Chunk.h"
#include "Generation/Generator.h"
#include "Entities/Entity.h"

enum WorldLayer;
enum BlockID;

class BlockEntity;

class Terrain : public Entity
{
	friend class World;
public:
	Terrain(World *world);
	~Terrain();
	
	// BLOCKS
	//bool setBlockAt(const int x, const int y, BlockID block, const WorldLayer layer);
	//BlockID getBlockAt(const int x, const int y, const WorldLayer layer);
	//bool isBlockAt(const int x, const int y, WorldLayer layer);

	// Removes the block at [x, y], creating an ItemDrop for the block type in the process
	//bool removeBlockAt(const int x, const int y, WorldLayer layer);

	// Static entities
	//void placeStaticEntity(BlockEntity *entity);

	// CHUNK LOADER
	///ChunkManager *getChunkManager() { return &m_chunkManager; }

private:
	// World pointer
	World *m_world;

	// CHUNK LOADER
	//ChunkManager m_chunkManager;
};

#endif // TERRAIN_H