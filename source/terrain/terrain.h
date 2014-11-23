#ifndef TERRAIN_H
#define TERRAIN_H

#include <x2d/x2d.h>
#include "terrainchunk.h"
#include "terraingen.h"

enum TerrainLayer;
enum BlockID;
extern TerrainLayer getLayerByTile(BlockID tile);

class Terrain
{
public:
	Terrain();
	
	// VERTEX FORMAT
	XVertexFormat getVertexFormat() const;
	
	// Move?
	void saveChunks();
	void load(const XIniFile &file);
	
	// TILE HELPERS
	BlockID getTileAt(const int x, const int y, const TerrainLayer layer);
	bool isTileAt(const int x, const int y, TerrainLayer layer);
	uint getTileState(const int x, const int y, TerrainLayer layer) /*const*/;
	
	// TILE MODIFICATION
	bool setTile(const int x, const int y, BlockID tile, const TerrainLayer layer);
	bool removeTile(const int x, const int y, TerrainLayer layer);
	
	// CHUNKS
	TerrainChunk &getChunk(const int chunkX, const int chunkY, const bool generate = false);
	void loadVisibleChunks();
	
	// UPDATING
	void update();
	
	// DRAWING
	void draw(const TerrainLayer layer, XBatch *batch);

private:
	// Terrain chunks
	//vector<TerrainChunk*> loadedChunks;
	list<TerrainChunk*> chunkLoadQueue;
	map<uint, TerrainChunk> chunks;
	XVertexFormat vertexFormat;
	
	// Terrain generator
	TerrainGen generator;
	
	// For selecting a direction to generate in
	Vector2 prevCameraPos;
};

#endif // TERRAIN_H