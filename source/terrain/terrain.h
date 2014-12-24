#ifndef TERRAIN_H
#define TERRAIN_H

#include <x2d/x2d.h>
#include "terrainchunk.h"
#include "generator/terraingen.h"

#include "game/gameobject.h"

enum TerrainLayer;
enum BlockID;

class Terrain : public GameObject, public xd::WindowListener
{
public:
	Terrain();
	~Terrain();
	
	// VERTEX FORMAT
	xd::VertexFormat getVertexFormat() const;
	
	// Move?
	void saveChunks();
	void load(const XIniFile &file);
	
	// BLOCK HELPERS
	BlockID getBlockAt(const int x, const int y, const TerrainLayer layer);
	bool isBlockAt(const int x, const int y, TerrainLayer layer);
	
	// BLOCK MODIFICATION
	bool setBlockAt(const int x, const int y, BlockID block, const TerrainLayer layer);
	bool removeBlockAt(const int x, const int y, TerrainLayer layer);
	
	// CHUNKS
	TerrainChunk &getChunk(const int chunkX, const int chunkY, const bool generate = false);
	bool isChunk(const int chunkX, const int chunkY) const;
	void loadVisibleChunks();
	
	// UPDATING
	void update();
	
	// DRAWING
	void draw(xd::SpriteBatch *spriteBatch);

	// WINDOW
	void resizeEvent(uint width, uint height);

private:
	// Terrain chunks
	//vector<TerrainChunk*> loadedChunks;
	TerrainChunk m_dummyChunk;
	list<TerrainChunk*> chunkLoadQueue;
	unordered_map<uint, TerrainChunk*> chunks;
	xd::VertexFormat vertexFormat;

	// SHADOWS
	xd::RenderTarget2D *m_shadowPass0;
	xd::RenderTarget2D *m_shadowPass1;
	xd::RenderTarget2D *m_shadowPass2;
	xd::RenderTarget2D *m_shadowRenderTarget;
	shared_ptr<xd::Shader> m_blurHShader;
	shared_ptr<xd::Shader> m_blurVShader;
	int m_shadowRadius;
	int m_prevX0, m_prevY0;
	
	// Terrain generator
	TerrainGen generator;
	
	// For selecting a direction to generate in
	Vector2 prevCameraPos;
};

#endif // TERRAIN_H