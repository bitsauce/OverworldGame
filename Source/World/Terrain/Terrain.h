#ifndef TERRAIN_H
#define TERRAIN_H

#include "Config.h"

#include "World/Terrain/ChunkLoader.h"
#include "World/Terrain/Chunk.h"
#include "Generation/Generator.h"

enum TerrainLayer;
enum BlockID;

class BlockEntity;

class Terrain : public WindowListener
{
	friend class World;
public:
	Terrain(World *world);
	~Terrain();
	
	// BLOCKS
	bool setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer);
	BlockID getBlockAt(const int x, const int y, const TerrainLayer layer);
	bool isBlockAt(const int x, const int y, TerrainLayer layer);

	// Removes the block at [x, y], creating an ItemDrop for the block type in the process
	bool removeBlockAt(const int x, const int y, TerrainLayer layer);

	// Static entities
	void placeStaticEntity(BlockEntity *entity);

	// CHUNK LOADER
	ChunkLoader *getChunkLoader() { return &m_chunkLoader; }

private:
	// World pointer
	World *m_world;

	// CHUNK LOADER
	ChunkLoader m_chunkLoader;
	
	// TERRAIN DRAWER
	class Drawer
	{
	public:
		// CONSTRUCTOR
		Drawer(Terrain *terrain, Camera *camera, const Priority drawOrder, const TerrainLayer layer);

		// DRAWING
		void draw(SpriteBatch *spriteBatch, const float alpha);

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