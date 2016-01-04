#ifndef TERRAIN_H
#define TERRAIN_H

#include "Config.h"

#include "Terrain/ChunkManager.h"
#include "Terrain/Chunk.h"
#include "Terrain/BlockDrawer.h"

#include "Generation/Generator.h"
#include "Entities/Entity.h"

enum WorldLayer;
enum BlockID;

class BlockEntity;

class Terrain : public Entity
{
	friend class World;
public:
	Terrain(World *world, Window *window);
	~Terrain();
	
	bool setBlockAt(const int x, const int y, BlockID block, const WorldLayer layer);
	BlockID getBlockAt(const int x, const int y, const WorldLayer layer);
	bool isBlockAt(const int x, const int y, WorldLayer layer);

	// Removes the block at [x, y], creating an ItemDrop for the block type in the process
	bool removeBlockAt(const int x, const int y, WorldLayer layer);

	void placeStaticEntity(BlockEntity *entity);

	ChunkManager *getChunkManager() { return m_chunkManager; }

private:
	World *m_world;
	ChunkManager *m_chunkManager;
};

#endif // TERRAIN_H