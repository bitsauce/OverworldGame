#ifndef TERRAIN_H
#define TERRAIN_H

#include <x2d/x2d.h>

#include "Game/GameObject.h"
#include "Terrain/ChunkLoader.h"
#include "Terrain/TerrainChunk.h"
#include "Terrain/Generator/TerrainGen.h"

enum TerrainLayer;
enum BlockID;

class Terrain : public GameObject, public xd::WindowListener
{
public:
	Terrain();
	~Terrain();
	
	// Move?
	void saveChunks();
	void load(const xd::IniFile &file);
	
	// BLOCK HELPERS
	BlockID getBlockAt(const int x, const int y, const TerrainLayer layer);
	bool isBlockAt(const int x, const int y, TerrainLayer layer);
	
	// BLOCK MODIFICATION
	bool setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer);
	bool removeBlockAt(const int x, const int y, TerrainLayer layer);
	
	// UPDATING
	void update();
	
	// DRAWING
	void draw(xd::SpriteBatch *spriteBatch);

	// WINDOW
	void resizeEvent(uint width, uint height);

	ChunkLoader *getChunkLoader() { return &m_chunkLoader; }

private:

	// CHUNKS
	ChunkLoader m_chunkLoader;
	
	// GENERATOR
	TerrainGen generator;
};

#endif // TERRAIN_H