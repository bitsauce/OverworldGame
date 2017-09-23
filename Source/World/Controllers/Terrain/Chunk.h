#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include "Config.h"
#include "Constants.h"
#include "Blocks/Block.h"

class LightSource;

// TODO: This class needs better encapsulation.
// While the friend class solution works, it is not pretty.
// Idealy, I want the user to only be able to manipulate the chunks through the Terrain class
class Chunk
{
	friend class Terrain;
	friend class ChunkManager;
	friend class BlockDrawer;
	friend class InGameDebug;
	friend class Pointlight;
	friend class Client;
	friend class Server;
private:
	Chunk(ChunkManager *chunkManager);
	Chunk(const Chunk &) = delete;

	/**
	 * \brief Loads \p blocks into the chunk at \p x, \p y
	 * \param chunkX The global chunk x-coordinate
	 * \param chunkY The global chunk y-coordinate
	 * \param blocks Array of blocks to load into the chunk
	 */
	void load(int chunkX, int chunkY, BlockID *blocks);

	/**
	 * \brief Unloads the chunk, deleting all block entities it holds
	 *        and updating the neighbour pointers of it's neighbours.
	 */
	void unload();

public:
	bool isAttached() const { return m_attached; }
	bool isSorted() const { return m_sorted; }
	bool isModified() const { return m_modified; }

	int getX() const { return m_x; }
	int getY() const { return m_y; }

	bool setBlockAt(const int x, const int y, const WorldLayer layer, const BlockID block);
	const BlockData *getBlockDataAt(const int x, const int y, const WorldLayer layer) const;
	BlockID getBlockAt(const int x, const int y, const WorldLayer layer) const;

	void addBlockEntity(BlockEntity *blockEntity);
	bool removeBlockEntity(BlockEntity *blockEntity);
	void setBlockEntityAt(const int x, const int y, const WorldLayer layer, BlockEntity *blockEntity);
	BlockEntity *getBlockEntityAt(const int x, const int y, const WorldLayer layer) const;
	bool setBlockEntityUVAt(const int x, const int y, const WorldLayer layer, const Vector2I &uvOffset);

	bool isEmptyAt(const int x, const int y, const WorldLayer layer) const;

	void attach(GraphicsContext *context, const int x, const int y);
	void detach();

	void drawBlockEntities(GraphicsContext *context);

private:
	tuple<BlockID, BlockEntity*> getNeighborBlock(const int x, const int y, const WorldLayer layer) const;

	// Chunk
	int m_x, m_y;
	BlockID *m_blocks;
	BlockEntity **m_blockEntities;
	list<BlockEntity*> m_blockEntityList;
	Chunk *m_neighborChunks[8];

	// Block entity VBO
	StaticVertexBuffer m_blockEntityVBO;
	StaticIndexBuffer m_blockEntityIBO;
	bool m_generateBlockEntityBuffers;

	// List of light sources that affect this chunk
	//list<LightSource*> m_lightSources;

	friend class Entity;
	list<Entity*> m_entities;

public: // TODO: private
	// Block entity time
	shared_ptr<Texture2D> m_blockEntitiesOffsetUVsTexture;

	// Chunk manager
	ChunkManager *m_chunkManager;

	// Block texture
	shared_ptr<Texture2D> m_blockTexture;

	// Flags
	bool m_modified;
	bool m_attached; //< attached = the block data of this chunk are rendered into the global render-target
	bool m_sorted;
};

#endif // TERRAIN_CHUNK_H