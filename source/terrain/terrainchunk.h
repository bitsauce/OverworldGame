#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include <x2d/x2d.h>

enum BlockID;
enum TerrainLayer;

enum ChunkState
{
	CHUNK_DUMMY,
	CHUNK_GENERATING,
	CHUNK_INITIALIZED
};

class TerrainChunk
{
	friend class Terrain;
public:
	TerrainChunk();
	TerrainChunk(int chunkX, int chunkY);
	
	// SERIALIZATION
	void load(int chunkX, int chunkY);
	void generate();
	void generateVBO();
	void serialize(XFileWriter &ss);
	void deserialize(stringstream &ss);
	
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	
	ChunkState getState() const;
	BlockID getBlockAt(const int x, const int y, TerrainLayer layer) const;
	xd::Texture2DPtr getLightMap() const { return m_shadowMap; }
	bool isBlockAt(const int x, const int y, TerrainLayer layer) const;
	bool isBlockOccupied(const int x, const int y, TerrainLayer layer) const;
	bool setBlockAt(const int x, const int y, const BlockID block, TerrainLayer layer);
	
	// DRAWING
	void draw(xd::GraphicsContext &gfxContext);

private:
	TerrainChunk(const TerrainChunk &) {}

	// PHYSICS
	void updateShadows();

	struct Block
	{
		Block();
		Block(BlockID id);
		BlockID id;
		Block *next[8];
	};
	
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
	Block **m_blocks;
	TerrainChunk *m_nextChunk[8];
	
	vector<BlockQuad> m_tmpQuads;
	static xd::Vertex *s_vertices;
	static uint *s_indices;
	static Block s_tempBlock;
	
	// DRAWING
	xd::StaticVertexBuffer m_vbo;
	xd::StaticIndexBuffer m_ibo;
	xd::Texture2DPtr m_shadowMap;
	
	// MISC
	bool m_modified;
	bool m_dirty;
	bool m_nextChunksGenerated;
	ChunkState m_state;
};

#endif // TERRAIN_CHUNK_H