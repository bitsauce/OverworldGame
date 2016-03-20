#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include "Config.h"
#include "Constants.h"

class BlockEntity;

/*class Block
{
public:
	Block(BlockID id) :
		m_block(id)
	{
	}

	bool isEntity()
	{
		return m_entity;
	}

private:
	BlockEntityData *m_data;
	BlockID m_block;
	bool m_entity;
};*/

class Chunk
{
	friend class ChunkManager;
public:
	Chunk(ChunkManager *chunkManager);

	/**
	 * \brief Loads \p blocks into the chunk at \p x, \p y
	 * \param chunkX The global chunk x-coordinate
	 * \param chunkY The global chunk y-coordinate
	 * \param blocks Array of blocks to load into the chunk
	 */
	void load(int chunkX, int chunkY, BlockID *blocks);
	
	bool isAttached() const { return m_attached; }
	bool isVisualized() const { return m_sorted; }
	bool isModified() const { return m_modified; }
	
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	
	BlockID getBlockAt(const int x, const int y, WorldLayer layer) const;
	bool isBlockAt(const int x, const int y, WorldLayer layer) const;
	bool isBlockOccupied(const int x, const int y, WorldLayer layer) const;
	bool setBlockAt(const int x, const int y, const BlockID block, WorldLayer layer);
	
	// DRAWING
	void attach(GraphicsContext *context, const int x, const int y);
	void detach();

private:
	Chunk(const Chunk &) { }

	// Chunk
	int m_x, m_y;
	BlockID *m_blocks;

	// Chunk manager
	ChunkManager *m_chunkManager;

	// Block texture
	Resource<Texture2D> m_blockTexture;

	// Flags
	bool m_modified;
	bool m_attached;
	bool m_sorted;
};

#endif // TERRAIN_CHUNK_H