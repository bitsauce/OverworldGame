#include "terrain.h"
#include "constants.h"
#include "game/debug.h"
#include "game/camera.h"
#include "game/world.h"

#define CHUNK_KEY(X, Y) ((X) & 0x0000FFFF) | (((Y) << 16) & 0xFFFF0000)

Terrain::Terrain() :
	GameObject(DRAW_ORDER_TERRAIN)
{
	LOG("Initializing terrain");
		
	// Setup vertex format
	vertexFormat.set(VERTEX_POSITION, 2);
	vertexFormat.set(VERTEX_TEX_COORD, 2);
		
	// Get terrain seed
	generator.seed = XRandom().nextInt();
}

// VERTEX FORMAT
XVertexFormat Terrain::getVertexFormat() const
{
	return vertexFormat;
}
	
// Move?
void Terrain::saveChunks()
{
	LOG("Saving chunks...");

	// Iterate loaded chunks
	for(unordered_map<uint, TerrainChunk*>::iterator itr = chunks.begin(); itr != chunks.end(); ++itr)
	{
		// Skip unmodified chunks
		if(!itr->second->m_modified) continue;

		// Save chunk
		string path = World::getWorldPath() + "/chunks/" + util::intToStr(CHUNK_KEY(itr->second->getX(), itr->second->getY())) + ".obj";
		XFileWriter writer(path);
		if(!writer)
		{
			LOG("Error opening chunk file: '%s'", path);
			continue;
		}
		itr->second->serialize(writer);
	}
}
	
void Terrain::load(const XIniFile &file)
{
	LOG("Loading terrain...");
		
	generator.seed = 0;// parseInt(file.getValue("terrain", "seed"));
}
	
// TILE HELPERS
BlockID Terrain::getBlockAt(const int x, const int y, const TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	return getChunk(XMath::floor(x / CHUNK_BLOCKSF), XMath::floor(y / CHUNK_BLOCKSF)).getTileAt(XMath::mod(x, CHUNK_BLOCKS), XMath::mod(y, CHUNK_BLOCKS), layer);
}
	
bool Terrain::isBlockAt(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	return getBlockAt(x, y, layer) != BLOCK_EMPTY;
}

// TILE MODIFICATION
bool Terrain::setTile(const int x, const int y, BlockID tile, const TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	if(getChunk(XMath::floor(x / CHUNK_BLOCKSF), XMath::floor(y / CHUNK_BLOCKSF)).setTile(XMath::mod(x, CHUNK_BLOCKS), XMath::mod(y, CHUNK_BLOCKS), tile, layer))
	{	
		// Update neighbouring tiles
		/*getChunk(XMath::floor(x     / CHUNK_BLOCKSF), XMath::floor(y     / CHUNK_BLOCKSF)).updateTile(XMath::mod(x,   CHUNK_BLOCKS), XMath::mod(y,   CHUNK_BLOCKS), getTileState(x,   y), true);
		getChunk(XMath::floor((x+1) / CHUNK_BLOCKSF), XMath::floor(y     / CHUNK_BLOCKSF)).updateTile(XMath::mod(x+1, CHUNK_BLOCKS), XMath::mod(y,   CHUNK_BLOCKS), getTileState(x+1, y), true);
		getChunk(XMath::floor((x-1) / CHUNK_BLOCKSF), XMath::floor(y     / CHUNK_BLOCKSF)).updateTile(XMath::mod(x-1, CHUNK_BLOCKS), XMath::mod(y,   CHUNK_BLOCKS), getTileState(x-1, y), true);
		getChunk(XMath::floor(x     / CHUNK_BLOCKSF), XMath::floor((y+1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x,   CHUNK_BLOCKS), XMath::mod(y+1, CHUNK_BLOCKS), getTileState(x, y+1), true);
		getChunk(XMath::floor(x     / CHUNK_BLOCKSF), XMath::floor((y-1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x,   CHUNK_BLOCKS), XMath::mod(y-1, CHUNK_BLOCKS), getTileState(x, y-1), true);
			
		getChunk(XMath::floor((x+1) / CHUNK_BLOCKSF), XMath::floor((y+1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x+1, CHUNK_BLOCKS), XMath::mod(y+1, CHUNK_BLOCKS), getTileState(x+1, y+1));
		getChunk(XMath::floor((x-1) / CHUNK_BLOCKSF), XMath::floor((y+1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x-1, CHUNK_BLOCKS), XMath::mod(y+1, CHUNK_BLOCKS), getTileState(x-1, y+1));
		getChunk(XMath::floor((x-1) / CHUNK_BLOCKSF), XMath::floor((y-1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x-1, CHUNK_BLOCKS), XMath::mod(y-1, CHUNK_BLOCKS), getTileState(x-1, y-1));
		getChunk(XMath::floor((x+1) / CHUNK_BLOCKSF), XMath::floor((y-1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x+1, CHUNK_BLOCKS), XMath::mod(y-1, CHUNK_BLOCKS), getTileState(x+1, y-1));*/
			
		return true;
	}
	return false;
}
	
bool Terrain::removeTile(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	if(getChunk(XMath::floor(x / CHUNK_BLOCKSF), XMath::floor(y / CHUNK_BLOCKSF)).setTile(XMath::mod(x, CHUNK_BLOCKS), XMath::mod(y, CHUNK_BLOCKS), BLOCK_EMPTY, layer))
	{
		// Update neighbouring tiles
		/*getChunk(XMath::floor(x     / CHUNK_BLOCKSF), XMath::floor(y     / CHUNK_BLOCKSF)).updateTile(XMath::mod(x,   CHUNK_BLOCKS), XMath::mod(y,   CHUNK_BLOCKS), getTileState(x,   y), true);
		getChunk(XMath::floor((x+1) / CHUNK_BLOCKSF), XMath::floor(y     / CHUNK_BLOCKSF)).updateTile(XMath::mod(x+1, CHUNK_BLOCKS), XMath::mod(y,   CHUNK_BLOCKS), getTileState(x+1, y), true);
		getChunk(XMath::floor((x-1) / CHUNK_BLOCKSF), XMath::floor(y     / CHUNK_BLOCKSF)).updateTile(XMath::mod(x-1, CHUNK_BLOCKS), XMath::mod(y,   CHUNK_BLOCKS), getTileState(x-1, y), true);
		getChunk(XMath::floor(x     / CHUNK_BLOCKSF), XMath::floor((y+1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x,   CHUNK_BLOCKS), XMath::mod(y+1, CHUNK_BLOCKS), getTileState(x, y+1), true);
		getChunk(XMath::floor(x     / CHUNK_BLOCKSF), XMath::floor((y-1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x,   CHUNK_BLOCKS), XMath::mod(y-1, CHUNK_BLOCKS), getTileState(x, y-1), true);
			
		getChunk(XMath::floor((x+1) / CHUNK_BLOCKSF), XMath::floor((y+1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x+1, CHUNK_BLOCKS), XMath::mod(y+1, CHUNK_BLOCKS), getTileState(x+1, y+1));
		getChunk(XMath::floor((x-1) / CHUNK_BLOCKSF), XMath::floor((y+1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x-1, CHUNK_BLOCKS), XMath::mod(y+1, CHUNK_BLOCKS), getTileState(x-1, y+1));
		getChunk(XMath::floor((x-1) / CHUNK_BLOCKSF), XMath::floor((y-1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x-1, CHUNK_BLOCKS), XMath::mod(y-1, CHUNK_BLOCKS), getTileState(x-1, y-1));
		getChunk(XMath::floor((x+1) / CHUNK_BLOCKSF), XMath::floor((y-1) / CHUNK_BLOCKSF)).updateTile(XMath::mod(x+1, CHUNK_BLOCKS), XMath::mod(y-1, CHUNK_BLOCKS), getTileState(x+1, y-1));*/
			
		return true;
	}
	return false;
}

// CHUNKS
TerrainChunk &Terrain::getChunk(const int chunkX, const int chunkY, const bool generate)
{
	uint key = CHUNK_KEY(chunkX, chunkY);
	if(chunks.find(key) == chunks.end())
	{
		if(generate)
		{
			LOG("Chunk [%i, %i] added to queue", chunkX, chunkY);
			
			// Create new chunk
			TerrainChunk *chunk = 0;
			string chunkFile = World::getWorldPath() + "/chunks/" + util::intToStr(key) + ".obj";
			if(util::fileExists(chunkFile))
			{
				//@chunk = cast<TerrainChunk>(@Scripts.deserialize(chunkFile));
			}
			else
			{
				chunk = new TerrainChunk(chunkX, chunkY);

				// Inform all neightbour chunks about the new guy
				if(isChunk(chunkX,   chunkY+1)) { chunks[CHUNK_KEY(chunkX,   chunkY+1)]->m_nextChunk[0] = chunk; chunk->m_nextChunk[4] = chunks[CHUNK_KEY(chunkX,   chunkY+1)]; }
				if(isChunk(chunkX-1, chunkY+1)) { chunks[CHUNK_KEY(chunkX-1, chunkY+1)]->m_nextChunk[1] = chunk; chunk->m_nextChunk[5] = chunks[CHUNK_KEY(chunkX-1, chunkY+1)]; }
				if(isChunk(chunkX-1, chunkY  )) { chunks[CHUNK_KEY(chunkX-1, chunkY  )]->m_nextChunk[2] = chunk; chunk->m_nextChunk[6] = chunks[CHUNK_KEY(chunkX-1, chunkY  )]; }
				if(isChunk(chunkX-1, chunkY-1)) { chunks[CHUNK_KEY(chunkX-1, chunkY-1)]->m_nextChunk[3] = chunk; chunk->m_nextChunk[7] = chunks[CHUNK_KEY(chunkX-1, chunkY-1)]; }
				if(isChunk(chunkX,   chunkY-1)) { chunks[CHUNK_KEY(chunkX,   chunkY-1)]->m_nextChunk[4] = chunk; chunk->m_nextChunk[0] = chunks[CHUNK_KEY(chunkX,   chunkY-1)]; }
				if(isChunk(chunkX+1, chunkY-1)) { chunks[CHUNK_KEY(chunkX+1, chunkY-1)]->m_nextChunk[5] = chunk; chunk->m_nextChunk[1] = chunks[CHUNK_KEY(chunkX+1, chunkY-1)]; }
				if(isChunk(chunkX+1, chunkY  )) { chunks[CHUNK_KEY(chunkX+1, chunkY  )]->m_nextChunk[6] = chunk; chunk->m_nextChunk[2] = chunks[CHUNK_KEY(chunkX+1, chunkY  )]; }
				if(isChunk(chunkX+1, chunkY+1)) { chunks[CHUNK_KEY(chunkX+1, chunkY+1)]->m_nextChunk[7] = chunk; chunk->m_nextChunk[3] = chunks[CHUNK_KEY(chunkX+1, chunkY+1)]; }
				//chunkLoadQueue.push_front(chunk); // Add to load queue
			}
				
			chunks[key] = chunk;
			chunk->generate();
			
			return *chunk;
		}
		return m_dummyChunk; // Create dummy
	}
	return *chunks[key];
}

bool Terrain::isChunk(const int chunkX, const int chunkY) const
{
	return chunks.find(CHUNK_KEY(chunkX, chunkY)) != chunks.end();
}
	
void Terrain::loadVisibleChunks()
{
	int x0 = XMath::floor(World::getCamera()->getX()/CHUNK_PXF);
	int y0 = XMath::floor(World::getCamera()->getY()/CHUNK_PXF);
	int x1 = XMath::floor((World::getCamera()->getX() + World::getCamera()->getWidth())/CHUNK_PXF);
	int y1 = XMath::floor((World::getCamera()->getY() + World::getCamera()->getHeight())/CHUNK_PXF);
		
	TerrainChunk *chunk;
	for(int y = y0; y <= y1; y++)
	{
		for(int x = x0; x <= x1; x++)
		{
			if((chunk = &getChunk(x, y, true))->getState() != CHUNK_INITIALIZED)
			{
				chunk->generate();
			}
		}
	}
	chunkLoadQueue.clear();
}
	
// UPDATING
void Terrain::update()
{
	if(XInput::getKeyState(XD_LMB))
	{
		setTile(XMath::floor((World::getCamera()->getPosition().x + XInput::getPosition().x)/BLOCK_PXF), XMath::floor((World::getCamera()->getPosition().y + XInput::getPosition().y)/BLOCK_PXF), BLOCK_SCENE_GRASS);
	}
	else if(XInput::getKeyState(XD_RMB))
	{
		setTile(XMath::floor((World::getCamera()->getPosition().x + XInput::getPosition().x)/BLOCK_PXF), XMath::floor((World::getCamera()->getPosition().y + XInput::getPosition().y)/BLOCK_PXF), BLOCK_EMPTY);
	}

	int cx = XMath::floor(World::getCamera()->getX()/CHUNK_PXF);
	int cy = XMath::floor(World::getCamera()->getY()/CHUNK_PXF);
	TerrainChunk *chunk = 0;
	if((chunk = &getChunk(cx, cy, true))->getState() != CHUNK_INITIALIZED)
	{
		LOG("Insta-generate chunk [%i, %i]", cx, cy);
		//chunkLoadQueue.remove(chunk);
		chunk->generate();
	}
		
	if(!chunkLoadQueue.empty())
	{
		// Load queued chunk
		chunkLoadQueue.back()->generate();
		chunkLoadQueue.pop_back();
	}

	Debug::setVariable("Chunks", util::intToStr(chunks.size()));
}
	
// DRAWING
void Terrain::draw(/*const TerrainLayer layer, */XBatch *batch)
{
	int x0 = XMath::floor(World::getCamera()->getX()/CHUNK_PXF);
	int y0 = XMath::floor(World::getCamera()->getY()/CHUNK_PXF);
	int x1 = XMath::floor((World::getCamera()->getX() + World::getCamera()->getWidth())/CHUNK_PXF);
	int y1 = XMath::floor((World::getCamera()->getY() + World::getCamera()->getHeight())/CHUNK_PXF);
		
	for(int y = y0; y <= y1; ++y)
	{
		for(int x = x0; x <= x1; ++x)
		{
			Matrix4 mat;
			mat.scale(BLOCK_PXF, BLOCK_PXF, 1.0f);
			mat.translate(x * CHUNK_PXF, y * CHUNK_PXF, 0.0f);

			batch->pushMatrix(mat);
			getChunk(x, y, true).draw(batch);
			batch->popMatrix();
		}
	}
	
	//if(XInput::getKeyState(XD_KEY_Z))
	{
		for(int y = y0; y <= y1; ++y)
		{
			XShape line(Rect(x0 * CHUNK_PX, y * CHUNK_PX, (x1 - x0 + 1) * CHUNK_PX, 1.0f / World::getCamera()->getZoom()));
			line.setFillColor(XColor(0, 0, 0, 255));
			line.draw(batch);
		}

		for(int x = x0; x <= x1; ++x)
		{
			XShape line(Rect(x * CHUNK_PX, y0 * CHUNK_PX, 1.0f / World::getCamera()->getZoom(), (y1 - y0 + 1) * CHUNK_PX));
			line.setFillColor(XColor(0, 0, 0, 255));
			line.draw(batch);
		}

		if(XInput::getKeyState(XD_KEY_B))
		{
			int x0 = XMath::floor(World::getCamera()->getX()/BLOCK_PXF);
			int y0 = XMath::floor(World::getCamera()->getY()/BLOCK_PXF);
			int x1 = XMath::floor((World::getCamera()->getX() + World::getCamera()->getWidth())/BLOCK_PXF);
			int y1 = XMath::floor((World::getCamera()->getY() + World::getCamera()->getHeight())/BLOCK_PXF);

			for(int y = y0; y <= y1; ++y)
			{
				XShape line(Rect(x0 * BLOCK_PXF, y * BLOCK_PXF, (x1 - x0 + 1) * BLOCK_PXF, 1.0f / World::getCamera()->getZoom()));
				line.setFillColor(XColor(0, 0, 0, 255));
				line.draw(batch);
			}

			for(int x = x0; x <= x1; ++x)
			{
				XShape line(Rect(x * BLOCK_PXF, y0 * BLOCK_PXF, 1.0f / World::getCamera()->getZoom(), (y1 - y0 + 1) * BLOCK_PXF));
				line.setFillColor(XColor(0, 0, 0, 255));
				line.draw(batch);
			} 
		}
	}
}