#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H

#include "Game/GameObject.h"
#include "Terrain/TerrainChunk.h"

class Camera;

class ChunkLoader : public GameObject, public xd::WindowListener
{
	friend class Debug;
public:
	ChunkLoader(Camera *camera);

	TerrainChunk &getChunkAt(const int chunkX, const int chunkY);
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

	void update();

private:
	TerrainChunk *loadChunkAt(const int chunkX, const int chunkY);
	
	void resizeEvent(uint width, uint height);

	bool m_applyZoom;
	Camera *m_camera;

	unordered_map<uint, TerrainChunk*> m_chunks;
	TerrainChunk m_dummyChunk;

	// Active area
	ChunkArea m_activeArea;

	// Load area
	ChunkArea m_loadArea;
	uint m_loadAreaRadius;

	// Chunk pool
	vector<TerrainChunk*> m_chunkPool;
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