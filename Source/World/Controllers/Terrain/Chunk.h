#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include "Config.h"
#include "Constants.h"

class BlockEntity;

class Chunk
{
	friend class ChunkManager;
public:
	Chunk(ChunkManager *chunkManager);

	// BLOCK LOADING
	void load(int chunkX, int chunkY, BlockID *blocks);
	
	bool isAttached() const { return m_attached; }
	bool isVisualized() const { return m_visualized; }
	bool isModified() const { return m_modified; }
	
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	
	BlockID getBlockAt(const int x, const int y, WorldLayer layer) const;
	bool isBlockAt(const int x, const int y, WorldLayer layer) const;
	bool isBlockOccupied(const int x, const int y, WorldLayer layer) const;
	bool setBlockAt(const int x, const int y, const BlockID block, WorldLayer layer);

	void update(const float dt);
	void onDraw(DrawEvent *e);

	void addStaticEntity(BlockEntity *entity);
	
	// DRAWING
	void attach(GraphicsContext *context, const int x, const int y);
	void detach();

private:
	Chunk(const Chunk &) { }

	// CHUNK
	int m_x, m_y;
	BlockID *m_blocks;

	// Chunk manager
	ChunkManager *m_chunkManager;

	// BLOCK ENTITIES
	set<BlockEntity*> m_blockEntities;

	// BLOCK GRID TEXTURE
	Resource<Texture2D> m_blockTexture;

	// MISC
	bool m_modified;
	bool m_attached;
	bool m_visualized; // m_sorted makes more sense.
};

#endif // TERRAIN_CHUNK_H