#ifndef TERRAIN_CHUNK_H
#define TERRAIN_CHUNK_H

#include <x2d/x2d.h>

enum TileID;
enum TerrainLayer;

enum ChunkState
{
	CHUNK_DUMMY,
	CHUNK_GENERATING,
	CHUNK_INITIALIZED
};

//Shader @blurHShader = @Shader(":/shaders/blur_h.vert", ":/shaders/blur_h.frag");
//Shader @blurVShader = @Shader(":/shaders/blur_v.vert", ":/shaders/blur_v.frag");

class TerrainTile
{
	int opCmp(const TerrainTile &other)
	{
		return tile - other.tile;
	}
	
	TileID tile;
	uint state;
	int x, y;
};

class TerrainChunk
{
	friend class TerrainManager;

public:
	TerrainChunk();
	TerrainChunk(int chunkX, int chunkY);
	
	// SERIALIZATION
	void init(int chunkX, int chunkY);
	void generate();
	void generateVBO();
	void serialize(stringstream &ss);
	void deserialize(stringstream &ss);
	
	int getX() const { return chunkX; }
	int getY() const { return chunkY; }
	
	ChunkState getState() const;
	TileID getTileAt(const int x, const int y, TerrainLayer layer) const;
	bool isTileAt(const int x, const int y, TerrainLayer layer) const;
	bool isReservedTileAt(const int x, const int y, TerrainLayer layer) const;
	bool setTile(const int x, const int y, const TileID tile, TerrainLayer layer);
	void updateTile(const int x, const int y, const uint tileState, const bool fixture = false);

	// SHADOWS
	float getOpacity(const int x, const int y);
	
	// DRAWING
	void draw(const Matrix4 &projmat);

private:
	// PHYSICS
	void createFixture(const int x, const int y);
	void removeFixture(const int x, const int y);
	bool isFixtureAt(const int x, const int y);
	void updateFixture(const int x, const int y, const uint state);
	void updateShadows();

	// CHUNK
	int chunkX, chunkY;
	//vector<vector<vector<TileID>>> tiles; TileID tiles[][][];
	
	// PHYSICS
	//b2Body *body;
	//grid<b2Fixture@> fixtures; b2Fixture *fixtures[][];
	
	// DRAWING
	XVertexBuffer vbo;
	XTexture *shadowMap;
	XTexture *shadowPass1;
	XTexture *shadowPass2;
	int shadowRadius;
	
	// MISC
	bool modified;
	bool dirty;
	ChunkState state;
		
};

#endif // TERRAIN_CHUNK_H