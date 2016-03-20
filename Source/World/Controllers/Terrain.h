#ifndef TERRAIN_H
#define TERRAIN_H

#include "Config.h"

#include "Terrain/ChunkManager.h"
#include "Terrain/Chunk.h"
#include "Terrain/BlockDrawer.h"
#include "Generation/Generator.h"
#include "Blocks/Block.h"

enum WorldLayer;
enum BlockID;

class Terrain : public SceneObject
{
	friend class World;
public:
	Terrain(World *world, Window *window);
	~Terrain();
	
	bool setBlockAt(const int x, const int y, const Block block, const WorldLayer layer);
	Block getBlockAt(const int x, const int y, const WorldLayer layer);
	bool isBlockAt(const int x, const int y, const WorldLayer layer);

	// Removes the block at [x, y], creating an ItemDrop for the block type in the process
	bool removeBlockAt(const int x, const int y, const WorldLayer layer);

	ChunkManager *getChunkManager() { return m_chunkManager; }

private:
	World *m_world;
	ChunkManager *m_chunkManager;
};

#endif // TERRAIN_H