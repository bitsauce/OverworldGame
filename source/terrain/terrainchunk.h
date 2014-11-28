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

class b2Body;
class b2Fixture;

//Shader @blurHShader = @Shader(":/shaders/blur_h.vert", ":/shaders/blur_h.frag");
//Shader @blurVShader = @Shader(":/shaders/blur_v.vert", ":/shaders/blur_v.frag");

class TerrainChunk
{
	friend class Terrain;
public:
	TerrainChunk();
	TerrainChunk(int chunkX, int chunkY);
	
	// SERIALIZATION
	void init(int chunkX, int chunkY);
	void generate();
	void generateVBO();
	void serialize(XFileWriter &ss);
	void deserialize(stringstream &ss);
	
	int getX() const { return m_x; }
	int getY() const { return m_y; }
	
	ChunkState getState() const;
	BlockID getTileAt(const int x, const int y, TerrainLayer layer) const;
	bool isTileAt(const int x, const int y, TerrainLayer layer) const;
	bool isBlockOccupied(const int x, const int y, TerrainLayer layer) const;
	bool setTile(const int x, const int y, const BlockID tile, TerrainLayer layer);
	void updateTile(const int x, const int y, const uint tileState, const bool fixture = false);

	// SHADOWS
	float getOpacity(const int x, const int y);
	
	// DRAWING
	void draw(XBatch *batch);

private:
	TerrainChunk(const TerrainChunk &) {}

	// PHYSICS
	void createFixture(const int x, const int y);
	void removeFixture(const int x, const int y);
	bool isFixtureAt(const int x, const int y);
	void updateFixture(const int x, const int y, const uint state);
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
		BlockID block;
		float x0, y0, x1, y1, u0, v0, u1, v1;
	};

	// CHUNK
	int m_x, m_y;
	Block **m_blocks;
	TerrainChunk *m_nextChunk[8];
	
	vector<BlockQuad> m_tmpQuads;
	static Block s_tempBlock;
	
	// PHYSICS
	b2Body *m_body;
	b2Fixture **m_fixtures;
	
	// DRAWING
	shared_ptr<XVertexBuffer> m_vbo;
	XTexture *m_shadowMap;
	XTexture *m_shadowPass1;
	XTexture *m_shadowPass2;
	int m_shadowRadius;
	
	// MISC
	bool m_modified;
	bool m_dirty;
	bool m_nextChunksGenerated;
	ChunkState m_state;
};

#endif // TERRAIN_CHUNK_H