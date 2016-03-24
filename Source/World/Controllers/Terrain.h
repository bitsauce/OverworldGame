#ifndef TERRAIN_H
#define TERRAIN_H

#include "Config.h"
#include "Terrain/ChunkManager.h"
#include "Terrain/Chunk.h"
#include "Terrain/BlockDrawer.h"
#include "Generation/Generator.h"
#include "Blocks/Block.h"

class Terrain : public SceneObject
{
	friend class World;
public:
	Terrain(World *world, Window *window);
	~Terrain();
	
	/**
	 * \brief Sets the block at global position \p x, \p y to \p block.
	 * \param x Global x position (in blocks)
	 * \param y Global y position (in blocks)
	 * \param layer The world layer
	 * \param block The block ID
	 * \param replace If true, the function will replace whatever is at the given position.
	 */
	bool setBlockAt(const int x, const int y, const WorldLayer layer, const BlockID blockID, const bool replace = false);
	
	/**
	 * \brief Returns the block ID at global position \p x, \p y.
	 * \param x Global x position (in blocks)
	 * \param y Global y position (in blocks)
	 * \param layer The world layer
	 * \return The block ID
	 */
	BlockID getBlockAt(const int x, const int y, const WorldLayer layer);
	
	/**
	 * \brief Returns true if the block at \p x, \p y is BLOCK_EMPTY.
	 * \param x Global x position (in blocks)
	 * \param y Global y position (in blocks)
	 * \param layer The world layer
	 */
	bool isBlockAt(const int x, const int y, const WorldLayer layer);
	
	/**
	 * \brief Removes the block at \p x, \p y
	 * \param x Global x position (in blocks)
	 * \param y Global y position (in blocks)
	 * \param layer The world layer
	 * \param createItem If true, this function will spawn an item drop at the position of the block
	 */
	bool removeBlockAt(const int x, const int y, const WorldLayer layer, const bool createItem = true);


	BlockEntity *createBlockEntityAt(const int x, const int y, const BlockEntityID blockEntity, const bool replace = false);

	BlockEntity *getBlockEntityAt(const int x, const int y, const WorldLayer layer);

	bool isBlockEntityAt(const int x, const int y, const WorldLayer layer);

	bool removeBlockEntityAt(const int x, const int y, const WorldLayer layer, const bool createItem = true);

	bool setBlockEntityFrameAt(const int x, const int y, const WorldLayer layer, const uint frame);


	bool isEmptyAt(const int x, const int y, const WorldLayer layer = WORLD_LAYER_MIDDLE);

	ChunkManager *getChunkManager() { return m_chunkManager; }

private:
	World *m_world;
	ChunkManager *m_chunkManager;
};

#endif // TERRAIN_H