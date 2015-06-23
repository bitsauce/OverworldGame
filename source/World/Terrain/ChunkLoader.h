#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H

#include "Config.h"
#include "World/Terrain/Chunk.h"

class Camera;
class World;
class Thing;
//>REMOVE ME<
class ChunkLoader : public WindowListener
{
	friend class Debug;
public:
	ChunkLoader(World *world);

	void clear();

	Chunk &getChunkAt(const int chunkX, const int chunkY);
	bool isChunkLoadedAt(const int chunkX, const int chunkY) const;

	void setOptimalChunkCount(const uint maxChunkCount);

	void loadActiveArea();
	
	struct ChunkArea
	{
		ChunkArea() : x0(0), y0(0), x1(0), y1(0) {}
		int x0, y0;
		int x1, y1;
	};

	ChunkArea getActiveArea() const;
	ChunkArea getLoadArea(/*const uint areaIndex*/) const;

	set<Thing*> getActiveThings();

	void update();

private:
	Chunk *loadChunkAt(const int chunkX, const int chunkY);
	
	void saveBlockData(const string &filePath, BlockID *blockData);
	void loadBlockData(const string &filePath, BlockID *blockData);
	bool freeInactiveChunk();
	void freeChunk(unordered_map<uint, Chunk*>::iterator itr);
	
	void resizeEvent(uint width, uint height);

	bool m_applyZoom;
	Camera *m_camera;
	WorldGenerator *m_generator;
	World *m_world;

	unordered_map<uint, Chunk*> m_chunks;
	Chunk m_dummyChunk;

	// Active area
	ChunkArea m_activeArea;

	// Load area
	ChunkArea m_loadArea;
	uint m_loadAreaRadius;

	// Chunk pool
	vector<Chunk*> m_chunkPool;
	uint m_optimalChunkCount;

	// Prev chunk pos
	Vector2i m_chunkPositions[4];
	Vector2  m_averagePosition;
	Vector2i m_prevChunkPosition;
	uint m_chunkPositionIndex;

	// Load patterns
	vector<Vector2i> m_circleLoadPattern;
	uint m_circleLoadIndex;
};

#endif // CHUNK_LOADER_H