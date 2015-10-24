#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include "Config.h"
#include "Constants.h"

class WorldGenerator;
class StaticEntity;

class Chunk
{
	friend class ChunkLoader;
public:
	Chunk(ChunkLoader *chunkLoader);

	// BLOCK LOADING
	void load(int chunkX, int chunkY, BlockID *blocks);
	
	// VBO LOADING
	bool isAttached() const { return m_attached; }
	bool isModified() const { return m_modified; }
	
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	
	BlockID getBlockAt(const int x, const int y, TerrainLayer layer) const;
	bool isBlockAt(const int x, const int y, TerrainLayer layer) const;
	bool isBlockOccupied(const int x, const int y, TerrainLayer layer) const;
	bool setBlockAt(const int x, const int y, const BlockID block, TerrainLayer layer);

	void addStaticEntity(StaticEntity *entity);
	
	// DRAWING
	void attach(GraphicsContext &context, const int x, const int y);
	void detach();

private:
	Chunk(const Chunk &) { }

	// CHUNK
	int m_x, m_y;
	BlockID *m_blocks;

	// CHUNK LOADER
	ChunkLoader *m_chunkLoader;

	// STATIC ENTITIES
	set<StaticEntity*> m_staticEntitites;

	// BLOCK GRID TEXTURE
	Texture2DPtr m_blockTexture;

	// MISC
	bool m_modified;
	bool m_attached;
};

#endif // TERRAIN_CHUNK_H