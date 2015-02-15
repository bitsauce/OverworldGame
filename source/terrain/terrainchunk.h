#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include "Config.h"
#include "Constants.h"

enum BlockID;
enum TerrainLayer;

class TerrainChunk
{
	friend class ChunkLoader;
public:
	TerrainChunk();

	// BLOCK LOADING
	void load(int chunkX, int chunkY);
	
	// VBO LOADING
	void generateVertexBuffer(ChunkLoader *chunkLoader, const TerrainLayer layer);
	bool isDirty(const TerrainLayer layer) const { return m_dirty[layer]; }

	// SERIALIZATION
	void serialize(FileWriter &ss);
	void deserialize(stringstream &ss);
	
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	
	BlockID getBlockAt(const int x, const int y, TerrainLayer layer) const;
	Texture2DPtr getLightMap() const { return m_shadowMap; }
	bool isBlockAt(const int x, const int y, TerrainLayer layer) const;
	bool isBlockOccupied(const int x, const int y, TerrainLayer layer) const;
	bool setBlockAt(const int x, const int y, const BlockID block, TerrainLayer layer);
	
	// DRAWING
	void draw(GraphicsContext &gfxContext, const TerrainLayer layer);

private:
	TerrainChunk(const TerrainChunk &) {}
	
	struct BlockQuad
	{
		BlockQuad();
		BlockQuad(BlockID block, const float x0, const float y0, const float x1, const float y1, const float u0, const float v0, const float u1, const float v1);
		bool operator<(const BlockQuad &other) const { return block < other.block; };
		bool operator>(const BlockQuad &other) const { return block > other.block; };
		BlockID block;
		float x0, y0, x1, y1, u0, v0, u1, v1;
	};

	// CHUNK
	int m_x, m_y;
	BlockID *m_blocks;

	// ADJACENCY LIST
	TerrainChunk *m_adjacentChunks[8];
	
	// VBO VARIABLES
	vector<BlockQuad> m_tmpQuads;
	static Vertex *s_vertices;
	static uint *s_indices;
	
	// VERTEX BUFFERS
	StaticVertexBuffer m_vertexBuffers[TERRAIN_LAYER_COUNT];
	StaticIndexBuffer m_indexBuffers[TERRAIN_LAYER_COUNT];

	// LIGHTING
	Texture2DPtr m_shadowMap;
	
	// MISC
	bool m_modified;
	bool m_dirty[TERRAIN_LAYER_COUNT];
};

#endif // TERRAIN_CHUNK_H