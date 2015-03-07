#ifndef TERRAIN_H
#define TERRAIN_H

#include "Config.h"

#include "Game/GameObject.h"
#include "Terrain/ChunkLoader.h"
#include "Terrain/Chunk.h"
#include "Generation/Generator.h"

enum TerrainLayer;
enum BlockID;

class Terrain : public WindowListener
{
public:
	Terrain(World &world);
	~Terrain();
	
	// BLOCKS
	bool setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer);
	BlockID getBlockAt(const int x, const int y, const TerrainLayer layer);
	bool isBlockAt(const int x, const int y, TerrainLayer layer);
	bool removeBlockAt(const int x, const int y, TerrainLayer layer);
	
	// BLOCK ENTITIES
	bool setThingAt(const int x, const int y, ThingID blockEntity);

	// CHUNK LOADER
	ChunkLoader *getChunkLoader() { return &m_chunkLoader; }

private:
	// CHUNK LOADER
	ChunkLoader m_chunkLoader;
	
	// TERRAIN DRAWER
	class Drawer : public GameObject
	{
	public:
		// CONSTRUCTOR
		Drawer(Terrain *terrain, Camera *camera, const Priority drawOrder, const TerrainLayer layer);

		// DRAWING
		void draw(SpriteBatch *spriteBatch);

	private:
		// CHUNK LOADER
		ChunkLoader *m_chunkLoader;

		// Camera
		Camera *m_camera;

		// LAYER
		TerrainLayer m_layer;
	};

	Drawer m_background;
	Drawer m_middleground;
	Drawer m_foreground;
};

#endif // TERRAIN_H