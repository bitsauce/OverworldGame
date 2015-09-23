#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include "Config.h"
#include "Constants.h"

class WorldGenerator;
class Thing;

class Chunk
{
	friend class ChunkLoader;
public:
	Chunk(ShaderPtr tileMapShader, ShaderPtr tileSortShader);

	// BLOCK LOADING
	void load(int chunkX, int chunkY, BlockID *blocks);
	
	// VBO LOADING
	void updateTileMap(ChunkLoader *chunkLoader, const TerrainLayer layer);
	bool isDirty(const TerrainLayer layer) const { return m_dirty[layer]; }
	bool isModified() const { return m_modified; }
	
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	
	BlockID getBlockAt(const int x, const int y, TerrainLayer layer) const;
	Texture2DPtr getLightMap() const { return m_shadowMap; }
	bool isBlockAt(const int x, const int y, TerrainLayer layer) const;
	bool isBlockOccupied(const int x, const int y, TerrainLayer layer) const;
	bool setBlockAt(const int x, const int y, const BlockID block, TerrainLayer layer);

	void addThing(Thing *thing) { m_things.push_back(thing); }//bool setThingAt(const int x, const int y, Thing *thing);
	void removeThing(Thing *thing) { m_things.remove(thing); }//Thing *getThingAt(const int x, const int y) const;
	list<Thing*> getThings() const { return m_things; }
	
	// DRAWING
	void draw(GraphicsContext &gfxContext, const TerrainLayer layer);

private:
	Chunk(const Chunk &) { }

	// CHUNK
	int m_x, m_y;
	BlockID *m_blocks;

	// ADJACENCY LIST
	Chunk *m_adjacentChunks[8];

	// TILE MAP SHADER
	ShaderPtr m_tileMapShader;
	Texture2DPtr m_tileMapTexture[TERRAIN_LAYER_COUNT];
	ShaderPtr m_tileSortShader;
	RenderTarget2D *m_sortRenderTarget[TERRAIN_LAYER_COUNT];

	// LIGHTING
	Texture2DPtr m_shadowMap;
	
	// THINGS
	list<Thing*> m_things;

	// MISC
	bool m_modified;
	bool m_dirty[TERRAIN_LAYER_COUNT];
	bool m_sorted[TERRAIN_LAYER_COUNT];
};

#endif // TERRAIN_CHUNK_H