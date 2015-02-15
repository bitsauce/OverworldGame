#ifndef TERRAIN_H
#define TERRAIN_H

#include "Config.h"

#include "Game/GameObject.h"
#include "Terrain/ChunkLoader.h"
#include "Terrain/TerrainChunk.h"
#include "Terrain/Generator/TerrainGen.h"

enum TerrainLayer;
enum BlockID;

class Terrain : public WindowListener
{
public:
	Terrain();
	~Terrain();
	
	// Move?
	void saveChunks();
	void load(const IniFile &file);
	
	// BLOCKS
	bool setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer);
	BlockID getBlockAt(const int x, const int y, const TerrainLayer layer);
	bool isBlockAt(const int x, const int y, TerrainLayer layer);
	bool removeBlockAt(const int x, const int y, TerrainLayer layer);
	
	// BLOCK ENTITIES
	bool setBlockEntityAt(const int x, const int y, BlockEntityID blockEntity);

	// CHUNK LOADER
	ChunkLoader *getChunkLoader() { return &m_chunkLoader; }

private:
	// CHUNK LOADER
	ChunkLoader m_chunkLoader;
	
	// GENERATOR
	TerrainGen generator;
	
	// TERRAIN DRAWER
	class Drawer : public GameObject
	{
	public:
		// CONSTRUCTOR
		Drawer(Terrain *terrain, const DrawOrder drawOrder, const TerrainLayer layer);

		// DRAWING
		void draw(SpriteBatch *spriteBatch);

	private:
		// CHUNK LOADER
		ChunkLoader *m_chunkLoader;

		// LAYER
		TerrainLayer m_layer;
	};

	Drawer m_background;
	Drawer m_middleground;
	Drawer m_foreground;
};

#endif // TERRAIN_H