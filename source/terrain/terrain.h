#ifndef TERRAIN_H
#define TERRAIN_H

#include <x2d/x2d.h>

#include "Game/GameObject.h"
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
	void load(const XIniFile &file);
	
	// BLOCK HELPERS
	BlockID getBlockAt(const int x, const int y, const TerrainLayer layer);
	bool isBlockAt(const int x, const int y, TerrainLayer layer);
	
	// BLOCK MODIFICATION
	bool setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer);
	bool removeBlockAt(const int x, const int y, TerrainLayer layer);
	
	// CHUNKS
	TerrainChunk &getChunk(const int chunkX, const int chunkY, const bool generate = false);
	bool isChunk(const int chunkX, const int chunkY) const;
	void loadVisibleChunks();
	void setMaxChunks(const uint maxChunkCount);
	
	// UPDATING
	void update();
	
	// DRAWING
	void draw(xd::SpriteBatch *spriteBatch);

	// WINDOW
	void resizeEvent(uint width, uint height);

private:
	// CHUNKS
	vector<TerrainChunk*> m_chunkPool;
	unordered_map<uint, TerrainChunk*> m_chunks;
	TerrainChunk m_dummyChunk;

	// SHADOWS
	int m_prevX0, m_prevY0;
	
	// GENERATOR
	TerrainGen generator;

	// For selecting a direction to generate in
	Vector2 prevCameraPos;
};

#endif // TERRAIN_H