#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H

#include "Game/GameObject.h"
#include "Terrain/TerrainChunk.h"

class Camera;

class ChunkLoader : public GameObject
{
	friend class Debug;
public:
	ChunkLoader(Camera *camera);

	TerrainChunk &getChunkAt(const int chunkX, const int chunkY, const bool generate = false);
	bool isChunkLoadedAt(const int chunkX, const int chunkY) const;

	void setMaxChunkCount(const uint maxChunkCount);
	void setOptimalChunkCount(const uint optimalChunkCount) { m_optimalChunkCount = optimalChunkCount; }

	void loadActiveArea();
	
	struct ChunkArea
	{
		int x0, y0;
		int x1, y1;
	};

	ChunkArea getActiveArea() const;
	ChunkArea getLoadArea(/*const uint areaIndex*/) const;

	void update();

private:
	TerrainChunk *loadChunkAt(const int chunkX, const int chunkY);

	bool m_applyZoom;
	Camera *m_camera;

	unordered_map<uint, TerrainChunk*> m_chunks;
	TerrainChunk m_dummyChunk;

	// Active area
	ChunkArea m_activeArea;
	Vector2i m_activeAreaSize;

	// Load area
	ChunkArea m_loadArea;

	uint m_maxChunkCount;
	uint m_optimalChunkCount;
	
	vector<TerrainChunk*> m_chunkPool;

	Vector2 m_prevCameraPos;
};

#endif // CHUNK_LOADER_H