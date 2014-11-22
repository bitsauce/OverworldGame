#include "terrainchunk.h"
#include "constants.h"
	
TerrainChunk::TerrainChunk()
{
	// A dummy
	state = CHUNK_DUMMY;
}
	
TerrainChunk::TerrainChunk(int chunkX, int chunkY)
{
	init(chunkX, chunkY);
}
	
// SERIALIZATION
void TerrainChunk::init(int chunkX, int chunkY)
{
	// Set chunk vars
	this->state = CHUNK_GENERATING;
	this->chunkX = chunkX;
	this->chunkY = chunkY;
	this->dirty = this->modified = false; // not modified
	this->shadowRadius = 4;
	this->shadowMap   = new XTexture(CHUNK_PX + shadowRadius*2, CHUNK_PX + shadowRadius*2);
	this->shadowPass1 = new XTexture(CHUNK_PX + shadowRadius*2, CHUNK_PX + shadowRadius*2);
	this->shadowPass2 = new XTexture(CHUNK_PX + shadowRadius*2, CHUNK_PX + shadowRadius*2);
	this->shadowPass2->setFiltering(XTexture::LINEAR);

	// access: x + (width * (y + depth * z))
	tiles = new TileID[CHUNK_BLOCKS*CHUNK_BLOCKS*TERRAIN_LAYER_COUNT];
		
	// Create body
	/*b2BodyDef def;
	def.type = b2_staticBody;
	def.position.set(chunkX * CHUNK_SIZE * TILE_PX, chunkY * CHUNK_SIZE * TILE_PX);
	def.allowSleep = true;
	@body = @b2Body(def);
	body.setObject(@Terrain);
		
	// Resize tile grid
	fixtures = grid<b2Fixture@>(CHUNK_SIZE, CHUNK_SIZE, null);
	tiles = array<grid<TileID>>(TERRAIN_LAYERS_MAX);
	for(int i = 0; i < TERRAIN_LAYERS_MAX; ++i)
	{
		tiles[i] = grid<TileID>(CHUNK_SIZE, CHUNK_SIZE, EMPTY_TILE);
	}*/
}
	
void TerrainChunk::generate()
{
	if(state == CHUNK_GENERATING)
	{
		// Set all tiles
		for(uint y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for(uint x = 0; x < CHUNK_BLOCKS; ++x)
			{
				for(int i = TERRAIN_LAYER_COUNT-1; i >= 0; --i)
				{
					tiles[x + (CHUNK_BLOCKS * (y + TERRAIN_LAYER_COUNT * i))] = TerrainGen::getTileAt(chunkX * CHUNK_BLOCKS + x, chunkY * CHUNK_BLOCKS + y, (TerrainLayer)i);
					//tiles[i][x, y] = Terrain.generator.getTileAt(chunkX * CHUNK_BLOCKS + x, chunkY * CHUNK_BLOCKS + y, TerrainLayer(i));
				}
			}
		}
		
		// Create vertex buffer
		generateVBO();
			
		// Re-generate neightbouring chunks
		/*Terrain.getChunk(chunkX+1, chunkY).dirty = true;
		Terrain.getChunk(chunkX+1, chunkY+1).dirty = true;
		Terrain.getChunk(chunkX,   chunkY+1).dirty = true;
		Terrain.getChunk(chunkX-1, chunkY+1).dirty = true;
		Terrain.getChunk(chunkX-1, chunkY).dirty = true;
		Terrain.getChunk(chunkX-1, chunkY-1).dirty = true;
		Terrain.getChunk(chunkX,   chunkY-1).dirty = true;
		Terrain.getChunk(chunkX+1, chunkY-1).dirty = true;*/
			
		// Mark chunk as initialized
		state = CHUNK_INITIALIZED;
		LOG("Chunk [%i, %i] generated", chunkX, chunkY);
	}
}
	
void TerrainChunk::generateVBO()
{
	// Create new vbo
	vbo = XVertexBuffer(/*Terrain.getVertexFormat()*/);
		
	// Load all vertex data
	vector<TerrainTile> sortedTiles;
	for(uint y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(uint x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(int i = TERRAIN_LAYER_COUNT-1; i >= 0; --i)
			{
				/*TileID tile = tiles[i][x, y];
				if(tile > RESERVED_TILE) // no point in updating air/reserved tiles
				{
					uint state = Terrain.getTileState(chunkX * CHUNK_BLOCKS + x, chunkY * CHUNK_BLOCKS + y, TerrainLayer(i));
						
					TerrainTile t;
					t.tile = tile;
					t.state = state;
					t.x = x;
					t.y = y;
					sortedTiles.push_back(t);
						
					updateFixture(x, y, state);
					//if(tileIsOpaque)
						break;
				}*/
			}
		}
	}
	//sortedTiles.sortAsc();
		
	for(int i = 0; i < sortedTiles.size(); ++i)
	{
		TerrainTile tile = sortedTiles[i];
		//vbo.addVertices(Tiles[tile.tile].getVertices(tile.x, tile.y, tile.state), Tiles[tile.tile].getIndices());
	}
			
	// Make the chunk buffer static
	vbo.setBufferType(XVertexBuffer::STATIC_BUFFER);
		
	updateShadows();
}
	
void TerrainChunk::serialize(XFileWriter &ss)
{
	LOG("Saving chunk [%i, %i]...", chunkX, chunkY);
		
	// Write chunk pos
	ss << chunkX << endl;
	ss << chunkY << endl;
		
	// Write chunk tiles
	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(int i = TERRAIN_LAYER_COUNT-1; i >= 0; --i)
			{
				/*TileID tile = tiles[i][x, y];
				if(tile <= RESERVED_TILE) tile = EMPTY_TILE;
				ss.write(int(tile));*/
			}
		}
	}
}
	
void TerrainChunk::deserialize(stringstream &ss)
{
	// Initialize chunk
	int chunkX, chunkY;
	ss >> chunkX;
	ss >> chunkY;
		
	LOG("Loading chunk [%i, %i]...", chunkX, chunkY);
		
	init(chunkX, chunkY);
		
	// Load tiles from file
	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(int i = TERRAIN_LAYER_COUNT-1; i >= 0; --i)
			{
				/*int tile;
				ss.read(tile);
				tiles[i][x, y] = TileID(tile);*/
			}
		}
	}
		
	state = CHUNK_INITIALIZED;
	generateVBO();
}
	
ChunkState TerrainChunk::getState() const
{
	return state;
}
	
TileID TerrainChunk::getTileAt(const int x, const int y, TerrainLayer layer) const
{
	return /*state != CHUNK_DUMMY ? tiles[layer][x, y] :*/ NULL_TILE;
}
	
bool TerrainChunk::isTileAt(const int x, const int y, TerrainLayer layer) const
{
	return /*state != CHUNK_DUMMY && tiles[layer][x, y] !=*/ EMPTY_TILE;
}
	
bool TerrainChunk::isReservedTileAt(const int x, const int y, TerrainLayer layer) const
{
	return /*state != CHUNK_DUMMY && tiles[layer][x, y] >*/ RESERVED_TILE;
}
	
bool TerrainChunk::setTile(const int x, const int y, const TileID tile, TerrainLayer layer)
{
	// Make sure we can add a tile here
	if(state == CHUNK_INITIALIZED/* && tiles[layer][x, y] != tile*/)
	{
		// Set the tile value
		//tiles[layer][x, y] = tile;
		dirty = modified = true; // mark chunk as modified
		return true; // return true as something was changed
	}
	return false; // nothing changed
}
	
void TerrainChunk::updateTile(const int x, const int y, const uint tileState, const bool fixture)
{
	if(state == CHUNK_INITIALIZED)
	{
		// Update shadow map
		/*float opacity = getOpacity(x, y);
		array<Vector4> pixel = { Vector4(0.0f, 0.0f, 0.0f, opacity) };
		shadowMap.updateSection(x + shadowRadius, CHUNK_SIZE - y - 1 + shadowRadius, Pixmap(1, 1, pixel));*/
			
		// Get tile
		/*TileID tile = tiles[x, y];
		int i = (y * CHUNK_SIZE + x) * 16;
		TextureRegion region;
		if(tile > RESERVED_TILE)
		{
			uint8 q1 = ((tileState >> 0) & 0x7) + 0x0;
			uint8 q2 = ((tileState >> 2) & 0x7) + 0x8;
			uint8 q3 = ((tileState >> 4) & 0x7) + 0x10;
			uint8 q4 = (((tileState >> 6) & 0x7) | ((tileState << 2) & 0x7)) + 0x18;
				
			array<Vertex> vertices = vbo.getVertices(i, 16);
				
			region = tileAtlas.get(tile, q1/32.0f, 0.0f, (q1+1)/32.0f, 1.0f);
			vertices[0].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
			vertices[1].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.y);
			vertices[2].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
			vertices[3].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
				
			region = tileAtlas.get(tile, q2/32.0f, 0.0f, (q2+1)/32.0f, 1.0f);
			vertices[4].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
			vertices[5].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.y);
			vertices[6].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
			vertices[7].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
				
			region = tileAtlas.get(tile, q3/32.0f, 0.0f, (q3+1)/32.0f, 1.0f);
			vertices[8].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
			vertices[9].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.y);
			vertices[10].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
			vertices[11].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
				
			region = tileAtlas.get(tile, q4/32.0f, 0.0f, (q4+1)/32.0f, 1.0f);
			vertices[12].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
			vertices[13].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.y);
			vertices[14].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
			vertices[15].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
				
			vbo.modifyVertices(i, vertices);
		}
		else
		{
			array<Vertex> vertices = vbo.getVertices(i, 16);
				
			vertices[0].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[1].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[2].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[3].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
				
			vertices[4].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[5].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[6].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[7].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
				
			vertices[8].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[9].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[10].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[11].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
				
			vertices[12].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[13].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[14].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[15].set4f(VERTEX_TEX_COORD, 0.0f, 0.0f);
				
			vbo.modifyVertices(i, vertices);
		}*/
			
		// Update fixtures
		if(fixture)
		{
			updateFixture(x, y, tileState);
		}
	}
}

// SHADOWS
float TerrainChunk::getOpacity(const int x, const int y)
{
	float opacity = 0.0f;
	for(int i = TERRAIN_LAYER_COUNT-1; i >= 0; --i)
	{
		opacity += 0.0f;//Tiles[getTileAt(x, y, TerrainLayer(i))].getOpacity();
	}
	return opacity;
}
	
// PHYSICS
void TerrainChunk::createFixture(const int x, const int y)
{
	//b2Fixture @fixture = @body.createFixture(Rect(x * TILE_PX - TILE_PX * 0.5f, y * TILE_PX - TILE_PX * 0.5f, TILE_PX*2, TILE_PX*2), 0.0f);
	/*b2Fixture @fixture = @body.createFixture(Rect(x * TILE_PX, y * TILE_PX - 3, TILE_PX, TILE_PX + 3), 0.0f);
	Tiles[getTileAt(x, y, TERRAIN_LAYER_SCENE)].setupFixture(@fixture);
	@fixtures[x, y] = @fixture;*/
}
	
void TerrainChunk::removeFixture(const int x, const int y)
{
	/*body.removeFixture(@fixtures[x, y]);
	@fixtures[x, y] = null;*/
}
	
bool TerrainChunk::isFixtureAt(const int x, const int y)
{
	return /*state != CHUNK_DUMMY ? @fixtures[x, y] != null :*/ false;
}
	
void TerrainChunk::updateFixture(const int x, const int y, const uint state)
{
	// Find out if this tile should contain a fixture
	bool shouldContainFixture = isReservedTileAt(x, y, TERRAIN_LAYER_SCENE) && (state & NESW) != NESW;
		
	// Create or remove fixture
	if(shouldContainFixture && !isFixtureAt(x, y))
	{
		createFixture(x, y);
	}
	else if(!shouldContainFixture && isFixtureAt(x, y))
	{
		removeFixture(x, y);
	}
}
	
void TerrainChunk::updateShadows()
{
	for(int y = -shadowRadius; y <= CHUNK_BLOCKS + shadowRadius; ++y)
	{
		for(int x = -shadowRadius; x <= CHUNK_BLOCKS + shadowRadius; ++x)
		{
			int tileX = chunkX*CHUNK_BLOCKS + x, tileY = chunkY*CHUNK_BLOCKS + y;
			/*float opacity = Terrain.getChunk(XMath::floor(tileX / CHUNK_BLOCKSF), XMath::floor(tileY / CHUNK_BLOCKSF)).getOpacity(XMath::mod(tileX, CHUNK_BLOCKS), XMath::mod(tileY, CHUNK_BLOCKS));
			array<Vector4> pixel = { Vector4(0.0f, 0.0f, 0.0f, opacity) };
			shadowMap.updateSection(x + shadowRadius, CHUNK_BLOCKS - y - 1 + shadowRadius, Pixmap(1, 1, pixel));*/
		}
	}
		
	XBatch batch;
		
	// Blur horizontally
	shadowPass1->clear();
	//blurHShader.setSampler2D("u_texture", @shadowMap);
	//blurHShader.setUniform1i("u_width", CHUNK_BLOCKS + shadowRadius*2);
	//blurHShader.setUniform1i("u_radius", shadowRadius-1);
	//batch.setShader(@blurHShader);
	XShape(Rect(0, 0, CHUNK_BLOCKS + shadowRadius*2, CHUNK_BLOCKS + shadowRadius*2)).draw(&batch);
	batch.renderToTexture(shadowPass1);
	batch.clear();
		
	// Blur vertically
	shadowPass2->clear();
	//blurVShader.setSampler2D("u_texture", @shadowPass1);
	//blurVShader.setSampler2D("u_filter", @shadowMap);
	//blurVShader.setUniform1i("u_height", CHUNK_BLOCKS + shadowRadius*2);
	//blurVShader.setUniform1i("u_radius", shadowRadius-1);
	//batch.setShader(@blurVShader);
	XShape(Rect(0, 0, CHUNK_BLOCKS + shadowRadius*2, CHUNK_BLOCKS + shadowRadius*2)).draw(&batch);
	batch.renderToTexture(shadowPass2);
	batch.clear();
}
	
// DRAWING
void TerrainChunk::draw(const Matrix4 &projmat)
{
	if(state == CHUNK_INITIALIZED)
	{
		if(dirty)
		{
			generateVBO();
			dirty = false;
		}
			
		// Draw tiles
		XBatch batch;
		batch.setProjectionMatrix(projmat);
		vbo.draw(&batch, 0/*@Tiles.getAtlas().getTexture()*/);
			
		if(XInput::getKeyState(XD_KEY_Z))
		{
			XShape line1(Rect(0, 0, CHUNK_PX, 1));
			line1.setFillColor(XColor(0, 0, 0, 255));
			line1.draw(&batch);
				
			XShape line2(Rect(0, 1, 1, CHUNK_PX));
			line2.setFillColor(XColor(0, 0, 0, 255));
			line2.draw(&batch);
		}
			
		XGraphics::renderBatch(batch);
		batch.clear();
			
		// Draw shadows
		float f = shadowRadius/(CHUNK_BLOCKSF + shadowRadius*2);
		XSprite shadows(XTextureRegion(shadowPass2, f, f, 1.0-f, 1.0-f));
		shadows.setPosition(CHUNK_PXF*chunkX, CHUNK_PXF*chunkY);
		shadows.setSize(CHUNK_PXF, CHUNK_PXF);
		//shadows.draw(Shadows);
	}
}