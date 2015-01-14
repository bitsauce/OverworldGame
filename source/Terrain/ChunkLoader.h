#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H

#include "Game/GameObject.h"
#include "Terrain/TerrainChunk.h"

class Camera;

class ChunkLoader : public GameObject
{
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

	ChunkArea getActiveArea();
	ChunkArea getLoadArea(const uint areaIndex);

	void update();

private:

	Camera *m_camera;

	unordered_map<uint, TerrainChunk*> m_chunks;
	TerrainChunk m_dummyChunk;

	ChunkArea m_activeArea;
	ChunkArea m_loadArea;

	uint m_maxChunkCount;
	uint m_optimalChunkCount;
	
	vector<TerrainChunk*> m_chunkPool;

	Vector2 m_prevCameraPos;
};

#endif // CHUNK_LOADER_H