#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H

#include "Config.h"
#include "Chunk.h"
#include "Entities/Entity.h"

class Camera;
class World;
class ChunkGenerator;

class ChunkManager : public GameObject
{
	friend class Debug;
	friend class Lighting;
	friend class BlockDrawer;
public:
	ChunkManager(World *world, Window *window);

	void clear();

	Chunk &getChunkAt(const int chunkX, const int chunkY);
	bool isChunkLoadedAt(const int chunkX, const int chunkY) const;

	void setOptimalChunkCount(const uint maxChunkCount);
	
	struct ChunkArea
	{
		ChunkArea() : x0(0), y0(0), x1(0), y1(0) {}

		int getWidth() const
		{
			return x1 - x0 + 1;
		}

		int getHeight() const
		{
			return y1 - y0 + 1;
		}

		bool operator==(const ChunkArea &other)
		{
			return x0 == other.x0 && y0 == other.y0 && x1 == other.x1 && y1 == other.y1;
		}

		bool operator!=(const ChunkArea &other)
		{
			return !(*this == other);
		}

		int x0, y0;
		int x1, y1;
	};

	ChunkArea getLoadingArea() const;
	ChunkArea getActiveArea() const;

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	void onWindowSizeChanged(WindowEvent *e);

private:
	Chunk *loadChunkAt(const int chunkX, const int chunkY);
	
	void saveBlockData(FileWriter &file, BlockID *blockData);
	void loadBlockData(FileReader &file, BlockID *blockData);

	void saveEntities(FileWriter &file, set<BlockEntity*> entities);
	void loadEntities(FileReader &file);

	bool freeInactiveChunk();
	void freeChunk(unordered_map<uint, Chunk*>::iterator itr);

	void updateViewSize(int width, int height);

	void reattachChunks(GraphicsContext *context);

	bool m_applyZoom;
	Window *m_window;
	Camera *m_camera;
	World *m_world;
	ChunkGenerator *m_generator;

	unordered_map<uint, Chunk*> m_chunks;

	// Active area
	ChunkArea m_activeArea;
	ChunkArea m_loadingArea;
	ChunkArea m_prevLoadingArea;
	bool m_redrawGlobalBlocks;

	// Loading area radius
	uint m_loadAreaRadius;

	// Chunk pool
	vector<Chunk*> m_chunkPool;
	uint m_optimalChunkCount;

	// Chunk positions
	Vector2i m_chunkPositions[4];
	Vector2  m_averagePosition;
	uint m_chunkPositionIndex;

	// Cirlce load patterns
	vector<Vector2i> m_circleLoadPattern;
	uint m_circleLoadIndex;

	// Block rendering shaders
	ShaderPtr m_tileSortShader;

	// Global render target
	RenderTarget2D *m_blocksRenderTarget;
	RenderTarget2D *m_sortedBlocksRenderTarget[WORLD_LAYER_COUNT];

	// Block rendering shader
	ShaderPtr m_tileMapShader;
	float m_time;

	// Lighting
	RenderTarget2D *m_lightingPass0;
	RenderTarget2D *m_lightingPass1;
	RenderTarget2D *m_lightingPass2;
	ShaderPtr m_directionalLightingShader;
	ShaderPtr m_radialLightingShader;
	ShaderPtr m_blurHShader;
	ShaderPtr m_blurVShader;
	int m_lightRadius;
	bool m_enabled;
	bool m_redrawLighting;
};

#endif // CHUNK_LOADER_H