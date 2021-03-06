#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H

#include "Config.h"
#include "Chunk.h"
#include "Entities/Entity.h"
#include "Blocks/Block.h"

class Camera;
class World;
class ChunkGenerator;

class ChunkManager : public SceneObject
{
	friend class Lighting;
	friend class InGameDebug;
	friend class BlockDrawer;
public:
	ChunkManager(World *world, Window *window);

	void updateLoadingAndActiveArea(const Vector2F &center);

	void clear();

	Chunk *getChunkAt(const int chunkX, const int chunkY, const bool loadChunk);
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
		int width, height;
	};

	ChunkArea getLoadingArea() const;
	ChunkArea getActiveArea() const;
	uint getLoadAreaRadius() const;

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	void onWindowSizeChanged(WindowEvent *e);

	RenderTarget2D *getBlocksRenderTarget() const
	{
		return m_blocksRenderTarget;
	}

	uint m_generatedChunks = 0;

	ChunkGenerator *getGenerator() const { return m_generator; }

private:
	Chunk *loadChunkAt(const int chunkX, const int chunkY);
	Chunk *popChunkFromPool();
	
	void saveBlockData(FileWriter &file, BlockID *blockData);
	void loadBlockData(FileReader &file, BlockID *blockData);

	void saveBlockEntities(FileWriter &file, list<BlockEntity*> entities);
	void loadBlockEntities(FileReader &file);

	void saveEntities(FileWriter &file, list<Entity*> entities);
	void loadEntities(FileReader &file);

	bool freeInactiveChunk();
	void freeChunk(unordered_map<uint, Chunk*>::iterator itr);

	void updateViewSize(int width, int height);

	void reattachChunk(Chunk *chunk, GraphicsContext *context);

	// Manager classes
	bool m_applyZoom;
	Window *m_window;
	Camera *m_camera;
	World *m_world;
	ChunkGenerator *m_generator;

	// Chunks
	unordered_map<uint, Chunk*> m_chunks;

	// Active area
	ChunkArea m_activeArea;
	ChunkArea m_loadingArea;
	ChunkArea m_prevLoadingArea;

	// Loading area radius (should be greater than 0)
	const uint m_loadAreaRadius;

	// Chunk pool
	list<Chunk*> m_chunkPool;
	uint m_optimalChunkCount;

	// Chunk positions
	Vector2I m_chunkPositions[4];
	uint m_chunkPositionIndex;

	// Cirlce load patterns
	vector<Vector2I> m_circleLoadPattern;
	uint m_circleLoadIndex;

	// Block rendering shaders
	Resource<Shader> m_tileSortShader;
	Resource<Shader> m_tileMapShader;
	Resource<Shader> m_blockEntityShader;
	float m_time;

	// Global render target
	RenderTarget2D *m_blocksRenderTarget;
	RenderTarget2D *m_sortedBlocksRenderTarget[WORLD_LAYER_COUNT];

	int m_lightRadius;
	bool m_enabled;
	bool m_redrawLighting;
};

#endif // CHUNK_LOADER_H