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
	Chunk(ShaderPtr tileMapShader, ShaderPtr tileSortShader);

	// BLOCK LOADING
	void load(int chunkX, int chunkY, BlockID *blocks);
	
	// VBO LOADING
	bool isAttached() const { return m_attached; }
	bool isModified() const { return m_modified; }
	
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	
	BlockID getBlockAt(const int x, const int y, TerrainLayer layer) const;
	Texture2DPtr getLightMap() const { return m_shadowMap; }
	bool isBlockAt(const int x, const int y, TerrainLayer layer) const;
	bool isBlockOccupied(const int x, const int y, TerrainLayer layer) const;
	bool setBlockAt(const int x, const int y, const BlockID block, TerrainLayer layer);

	void addStaticEntity(StaticEntity *entity);
	
	// DRAWING
	void attachBlocks(GraphicsContext &gfxContext, ChunkLoader *chunkLoader);
	void detachBlocks();

private:
	Chunk(const Chunk &) { }
	void updateBlockGridTexture(ChunkLoader *chunkLoader);

	// CHUNK
	int m_x, m_y;
	BlockID *m_blocks;

	// STATIC ENTITIES
	set<StaticEntity*> m_staticEntitites;

	// ADJACENCY LIST
	Chunk *m_adjacentChunks[8];

	// BLOCK GRID TEXTURE
	Texture2DPtr m_blockGridTexture;

	// LIGHTING
	Texture2DPtr m_shadowMap;

	// MISC
	bool m_modified;
	bool m_attached;
};

#endif // TERRAIN_CHUNK_H