#include "terrainchunk.h"
#include "terrain.h"
#include "terraingen.h"
#include "constants.h"

#include "game/world.h"
#include "game/blockdata.h"

#include <Box2D/Box2D.h>
	
TerrainChunk::TerrainChunk()
{
	// A dummy
	m_state = CHUNK_DUMMY;

	// Init neightbour chunks
	for(uint i = 0; i < 8; ++i)
	{
		m_nextChunk[i] = nullptr;
	}
}
	
TerrainChunk::TerrainChunk(int chunkX, int chunkY)
{
	init(chunkX, chunkY);
}

#define BLOCK_INDEX(x, y, z) x + CHUNK_BLOCKS * (y + CHUNK_BLOCKS * z)
#define FIXTURE_INDEX(x, y) x + CHUNK_BLOCKS * y
	
// SERIALIZATION
void TerrainChunk::init(int chunkX, int chunkY)
{
	// Set chunk vars
	m_state = CHUNK_GENERATING;
	m_x = chunkX;
	m_y = chunkY;
	m_dirty = m_modified = false; // not modified
	m_shadowRadius = 4;
	m_shadowMap   = new XTexture(CHUNK_PX + m_shadowRadius*2, CHUNK_PX + m_shadowRadius*2);
	m_shadowPass1 = new XTexture(CHUNK_PX + m_shadowRadius*2, CHUNK_PX + m_shadowRadius*2);
	m_shadowPass2 = new XTexture(CHUNK_PX + m_shadowRadius*2, CHUNK_PX + m_shadowRadius*2);
	m_shadowPass2->setFiltering(XTexture::LINEAR);

	// Initialize tiles
	m_blocks = new BlockID[CHUNK_BLOCKS*CHUNK_BLOCKS*TERRAIN_LAYER_COUNT];
	for(uint z = 0; z < TERRAIN_LAYER_COUNT; ++z)
	{
		for(uint y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for(uint x = 0; x < CHUNK_BLOCKS; ++x)
			{
				m_blocks[BLOCK_INDEX(x, y, z)] = BLOCK_EMPTY;
			}
		}
	}

	// Init neightbour chunks
	for(uint i = 0; i < 8; ++i)
	{
		m_nextChunk[i] = nullptr;
	}

	// Create body
	b2BodyDef def;
	def.type = b2_staticBody;
	def.position.Set(chunkX * CHUNK_PXF, chunkY * CHUNK_PXF);
	def.allowSleep = true;
	
	m_body = World::getb2World()->CreateBody(&def);
	m_body->SetUserData(World::getTerrain());
		
	// Resize tile grid
	m_fixtures = new b2Fixture*[CHUNK_BLOCKS*CHUNK_BLOCKS];
	for(uint y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(uint x = 0; x < CHUNK_BLOCKS; ++x)
		{
			m_fixtures[FIXTURE_INDEX(x, y)] = nullptr;
		}
	}
}
	
void TerrainChunk::generate()
{
	if(m_state == CHUNK_GENERATING)
	{
		// Set all tiles
		for(uint z = 0; z < TERRAIN_LAYER_COUNT; ++z)
		{
			for(uint y = 0; y < CHUNK_BLOCKS; ++y)
			{
				for(uint x = 0; x < CHUNK_BLOCKS; ++x)
				{
					m_blocks[BLOCK_INDEX(x, y, z)] = TerrainGen::getTileAt(m_x * CHUNK_BLOCKS + x, m_y * CHUNK_BLOCKS + y, (TerrainLayer)z);
				}
			}
		}
		
		// Create vertex buffer
		generateVBO();
			
		// Re-generate neightbouring chunks
		if(m_nextChunk[0]) m_nextChunk[0]->m_dirty = true;
		if(m_nextChunk[1]) m_nextChunk[1]->m_dirty = true;
		if(m_nextChunk[2]) m_nextChunk[2]->m_dirty = true;
		if(m_nextChunk[3]) m_nextChunk[3]->m_dirty = true;
		if(m_nextChunk[4]) m_nextChunk[4]->m_dirty = true;
		if(m_nextChunk[5]) m_nextChunk[5]->m_dirty = true;
		if(m_nextChunk[6]) m_nextChunk[6]->m_dirty = true;
		if(m_nextChunk[7]) m_nextChunk[7]->m_dirty = true;
			
		// Mark chunk as initialized
		m_state = CHUNK_INITIALIZED;
		LOG("Chunk [%i, %i] generated", m_x, m_y);
	}
}
	
void TerrainChunk::generateVBO()
{
	// Create new vbo
	m_vbo = shared_ptr<XVertexBuffer>(new XVertexBuffer(World::getTerrain()->getVertexFormat()));
		
	// Load all vertex data
	for(uint y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(uint x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(int i = TERRAIN_LAYER_COUNT-1; i >= 0; --i)
			{
				BlockID blocks[9];
				World::getTerrain()->getTileState(m_x * CHUNK_BLOCKS + x, m_y * CHUNK_BLOCKS + y, blocks, (TerrainLayer)i);
				BlockData::get(blocks[0]).getVertices(x, y, blocks, m_vbo);

				//if(block > BLOCK_RESERVED) // no point in updating air/reserved tiles
				{
					//updateFixture(x, y, state);
				}
				//if(tileIsOpaque) break;
			}
		}
	}
	
	// Make the chunk buffer static
	m_vbo->setBufferType(XVertexBuffer::STATIC_BUFFER);
		
	updateShadows();
}
	
void TerrainChunk::serialize(XFileWriter &ss)
{
	LOG("Saving chunk [%i, %i]...", m_x, m_y);
		
	// Write chunk pos
	ss << m_x << endl;
	ss << m_y << endl;
		
	// Write chunk tiles
	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(int i = TERRAIN_LAYER_COUNT-1; i >= 0; --i)
			{
				BlockID block = m_blocks[BLOCK_INDEX(x, y, i)];
				if(block <= BLOCK_RESERVED) block = BLOCK_EMPTY;
				ss << block << endl;
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
				int tile;
				ss >> tile;
				m_blocks[BLOCK_INDEX(x, y, i)] = BlockID(tile);
			}
		}
	}
		
	m_state = CHUNK_INITIALIZED;
	generateVBO();
}
	
ChunkState TerrainChunk::getState() const
{
	return m_state;
}
	
BlockID TerrainChunk::getTileAt(const int x, const int y, TerrainLayer layer) const
{
	return m_state != CHUNK_DUMMY ? m_blocks[BLOCK_INDEX(x, y, layer)] : BLOCK_EMPTY;
}
	
bool TerrainChunk::isTileAt(const int x, const int y, TerrainLayer layer) const
{
	return m_state != CHUNK_DUMMY && m_blocks[BLOCK_INDEX(x, y, layer)] != BLOCK_EMPTY;
}
	
bool TerrainChunk::isReservedTileAt(const int x, const int y, TerrainLayer layer) const
{
	return m_state != CHUNK_DUMMY && m_blocks[BLOCK_INDEX(x, y, layer)] > BLOCK_RESERVED;
}
	
bool TerrainChunk::setTile(const int x, const int y, const BlockID tile, TerrainLayer layer)
{
	// Make sure we can add a tile here
	if(m_state == CHUNK_INITIALIZED && m_blocks[BLOCK_INDEX(x, y, layer)] != tile)
	{
		// Set the tile value
		m_blocks[BLOCK_INDEX(x, y, layer)] = tile;
		m_dirty = m_modified = true; // mark chunk as modified
		if(m_nextChunk[0] && y == 0)										m_nextChunk[0]->m_dirty = true;
		if(m_nextChunk[1] && x == CHUNK_BLOCKS-1 && y == 0)					m_nextChunk[1]->m_dirty = true;
		if(m_nextChunk[2] && x == CHUNK_BLOCKS-1)							m_nextChunk[2]->m_dirty = true;
		if(m_nextChunk[3] && x == CHUNK_BLOCKS-1 && y == CHUNK_BLOCKS-1)	m_nextChunk[3]->m_dirty = true;
		if(m_nextChunk[4] && y == CHUNK_BLOCKS-1)							m_nextChunk[4]->m_dirty = true;
		if(m_nextChunk[5] && x == 0 && y == CHUNK_BLOCKS-1)					m_nextChunk[5]->m_dirty = true;
		if(m_nextChunk[6] && x == 0)										m_nextChunk[6]->m_dirty = true;
		if(m_nextChunk[7] && x == 0 && y == 0)								m_nextChunk[7]->m_dirty = true;
		return true; // return true as something was changed
	}
	return false; // nothing changed
}
	
void TerrainChunk::updateTile(const int x, const int y, const uint tileState, const bool fixture)
{
	if(m_state == CHUNK_INITIALIZED)
	{
			
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

b2Vec2 tob2Vec(const Vector2 &vec)
{
	return b2Vec2(vec.x, vec.y);
}
	
// PHYSICS
void TerrainChunk::createFixture(const int x, const int y)
{
	//b2Fixture @fixture = @body.createFixture(Rect(x * TILE_PX - TILE_PX * 0.5f, y * TILE_PX - TILE_PX * 0.5f, TILE_PX*2, TILE_PX*2), 0.0f);
	Rect rect(x * BLOCK_PX, y * BLOCK_PX - 3, BLOCK_PX, BLOCK_PX + 3);

	b2PolygonShape shape;
	shape.SetAsBox(rect.getHeight()*0.5f, rect.getHeight()*0.5f, tob2Vec(rect.getCenter()), 0.0f);

	b2FixtureDef def;
	def.shape = &shape;
	def.density = 0.0f;

	b2Fixture *fixture = m_body->CreateFixture(&def);
	BlockData::get(getTileAt(x, y, TERRAIN_LAYER_SCENE)).setupFixture(fixture);
	m_fixtures[FIXTURE_INDEX(x, y)] = fixture;
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
	for(int y = -m_shadowRadius; y <= CHUNK_BLOCKS + m_shadowRadius; ++y)
	{
		for(int x = -m_shadowRadius; x <= CHUNK_BLOCKS + m_shadowRadius; ++x)
		{
			int tileX = m_x*CHUNK_BLOCKS + x, tileY = m_y*CHUNK_BLOCKS + y;
			/*float opacity = Terrain.getChunk(XMath::floor(tileX / CHUNK_BLOCKSF), XMath::floor(tileY / CHUNK_BLOCKSF)).getOpacity(XMath::mod(tileX, CHUNK_BLOCKS), XMath::mod(tileY, CHUNK_BLOCKS));
			array<Vector4> pixel = { Vector4(0.0f, 0.0f, 0.0f, opacity) };
			shadowMap.updateSection(x + shadowRadius, CHUNK_BLOCKS - y - 1 + shadowRadius, Pixmap(1, 1, pixel));*/
		}
	}
		
	XBatch batch;
		
	// Blur horizontally
	m_shadowPass1->clear();
	//blurHShader.setSampler2D("u_texture", @shadowMap);
	//blurHShader.setUniform1i("u_width", CHUNK_BLOCKS + shadowRadius*2);
	//blurHShader.setUniform1i("u_radius", shadowRadius-1);
	//batch.setShader(@blurHShader);
	XShape(Rect(0, 0, CHUNK_BLOCKS + m_shadowRadius*2, CHUNK_BLOCKS + m_shadowRadius*2)).draw(&batch);
	batch.renderToTexture(m_shadowPass1);
	batch.clear();
		
	// Blur vertically
	m_shadowPass2->clear();
	//blurVShader.setSampler2D("u_texture", @shadowPass1);
	//blurVShader.setSampler2D("u_filter", @shadowMap);
	//blurVShader.setUniform1i("u_height", CHUNK_BLOCKS + shadowRadius*2);
	//blurVShader.setUniform1i("u_radius", shadowRadius-1);
	//batch.setShader(@blurVShader);
	XShape(Rect(0, 0, CHUNK_BLOCKS + m_shadowRadius*2, CHUNK_BLOCKS + m_shadowRadius*2)).draw(&batch);
	batch.renderToTexture(m_shadowPass2);
	batch.clear();
}
	
// DRAWING
void TerrainChunk::draw(XBatch *batch)
{
	if(m_state == CHUNK_INITIALIZED)
	{
		if(m_dirty)
		{
			generateVBO();
			m_dirty = false;
		}
		
		// Draw blocks
		batch->setTexture(BlockData::s_blockAtlas->getTexture());
		batch->setPrimitive(XBatch::PRIMITIVE_TRIANGLES);
		batch->addVertexBuffer(m_vbo);
		
		// Draw shadows
		/*float f = m_shadowRadius/(CHUNK_BLOCKSF + m_shadowRadius*2);
		XSprite shadows(XTextureRegion(shared_ptr<XTexture>(m_shadowPass2), f, f, 1.0-f, 1.0-f));
		shadows.setPosition(CHUNK_PXF*m_x, CHUNK_PXF*m_y);
		shadows.setSize(CHUNK_PXF, CHUNK_PXF);*/
		//shadows.draw(Shadows);
	}
}