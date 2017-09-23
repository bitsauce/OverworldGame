#ifndef TERRAIN_H
#define TERRAIN_H

#include "Config.h"
#include "ChunkManager.h"
#include "Chunk.h"
#include "BlockDrawer.h"
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
	bool setBlockAt(const int x, const int y, const WorldLayer layer, const BlockData *block, const bool replace = false);
	
	/**
	 * \brief Returns the block ID at global position \p x, \p y.
	 * \param x Global x position (in blocks)
	 * \param y Global y position (in blocks)
	 * \param layer The world layer
	 * \return The block ID
	 */
	//const BlockData *getBlockAt(const int x, const int y, const WorldLayer layer, const BlockEntity **blockEntity = nullptr);

	const BlockData *getBlockAt(const int x, const int y, const WorldLayer layer);
	
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

	/**
	 * \brief Creates a block entity at the global position \p x, \p y.
	 * \param x Global x position (in blocks)
	 * \param y Global y position (in blocks)
	 * \param blockEntity The block ID
	 * \param replace If true, the function will replace whatever is at the given position.
	 */
	BlockEntity *createBlockEntityAt(const int x, const int y, const BlockEntityData *blockEntityData, const bool replace = false);

	BlockEntity *getBlockEntityAt(const int x, const int y, const WorldLayer layer);

	bool isBlockEntityAt(const int x, const int y, const WorldLayer layer);

	bool removeBlockEntityAt(const int x, const int y, const WorldLayer layer, const bool createItem = true);

	bool setBlockEntityUVAt(const int x, const int y, const WorldLayer layer, const Vector2F &uv);


	bool isEmptyAt(const int x, const int y, const WorldLayer layer = WORLD_LAYER_MIDDLE);

	ChunkManager *getChunkManager() { return m_chunkManager; }

private:
	World *m_world;
	ChunkManager *m_chunkManager;
};

#endif // TERRAIN_H