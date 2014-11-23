#include "terrain.h"
#include "constants.h"
#include "game/debug.h"
#include "game/camera.h"
#include "game/world.h"

#define CHUNK_KEY(chunkX, chunkY) (chunkX & 0x0000FFFF) | ((chunkY << 16) & 0xFFFF0000)

TerrainLayer getLayerByTile(BlockID tile)
{
	if(tile < BACKGROUND_BLOCKS) return TERRAIN_LAYER_BACKGROUND;
	if(tile < SCENE_BLOCKS) return TERRAIN_LAYER_SCENE;
	return TERRAIN_LAYER_FOREGROUND;
}

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
	for(map<uint, TerrainChunk*>::iterator itr = chunks.begin(); itr != chunks.end(); ++itr)
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
BlockID Terrain::getTileAt(const int x, const int y, const TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	return getChunk(XMath::floor(x / CHUNK_PXF), XMath::floor(y / CHUNK_PXF)).getTileAt(XMath::mod(x, CHUNK_PX), XMath::mod(y, CHUNK_PX), layer);
}
	
bool Terrain::isTileAt(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	return getTileAt(x, y, layer) > BLOCK_RESERVED;
}
	
uint Terrain::getTileState(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	// Get state
	uint state = 0;
	int chunkX = XMath::floor(x / CHUNK_PXF), chunkY = XMath::floor(y / CHUNK_PXF), tileX = XMath::mod(x, CHUNK_PX), tileY = XMath::mod(y, CHUNK_PX);
	TerrainChunk *chunk = &getChunk(chunkX, chunkY), *chunkN, *chunkS;
	if(chunk->getState() == CHUNK_DUMMY) return 0;
		
	BlockID tile = chunk->getTileAt(tileX, tileY, layer);
	if(tileY == 0)
	{
		chunkN = &getChunk(chunkX, chunkY-1);
		if(chunkN->getTileAt(tileX, CHUNK_BLOCKS-1, layer) == tile) state |= NORTH;
		if(chunk->getTileAt(tileX, tileY+1, layer) == tile) state |= SOUTH;
	}
	else if(tileY == CHUNK_BLOCKS-1)
	{
		chunkS = &getChunk(chunkX, chunkY+1);
		if(chunkS->getTileAt(tileX, 0, layer) == tile) state |= SOUTH;
		if(chunk->getTileAt(tileX, tileY-1, layer) == tile) state |= NORTH;
	}
	else
	{
		if(chunk->getTileAt(tileX, tileY-1, layer) == tile) state |= NORTH;
		if(chunk->getTileAt(tileX, tileY+1, layer) == tile) state |= SOUTH;
	}
		
	if(tileX == 0)
	{
		TerrainChunk *chunkW = &getChunk(chunkX-1, chunkY);
		if(chunkW->getTileAt(CHUNK_BLOCKS-1, tileY, layer) == tile) state |= WEST;
		if(tileY == 0)
		{
			if(getChunk(chunkX-1, chunkY-1).getTileAt(CHUNK_BLOCKS-1, CHUNK_BLOCKS-1, layer) == tile) state |= NORTH_WEST;
			if(chunkW->getTileAt(CHUNK_BLOCKS-1, tileY+1, layer) == tile) state |= SOUTH_WEST;
			if(chunkN->getTileAt(tileX+1, CHUNK_BLOCKS-1, layer) == tile) state |= NORTH_EAST;
			if(chunk->getTileAt(tileX+1, tileY+1, layer) == tile) state |= SOUTH_EAST;
		}
		else if(tileY == CHUNK_BLOCKS-1)
		{
			if(getChunk(chunkX-1, chunkY+1).getTileAt(CHUNK_BLOCKS-1, 0, layer) == tile) state |= SOUTH_WEST;
			if(chunkW->getTileAt(CHUNK_BLOCKS-1, tileY-1, layer) == tile) state |= NORTH_WEST;
			if(chunk->getTileAt(tileX+1, tileY-1, layer) == tile) state |= NORTH_EAST;
			if(chunkS->getTileAt(tileX+1, 0, layer) == tile) state |= SOUTH_EAST;
		}
		else
		{
			if(chunkW->getTileAt(CHUNK_BLOCKS-1, tileY-1, layer) == tile) state |= NORTH_WEST;
			if(chunkW->getTileAt(CHUNK_BLOCKS-1, tileY+1, layer) == tile) state |= SOUTH_WEST;
			if(chunk->getTileAt(tileX+1, tileY-1, layer) == tile) state |= NORTH_EAST;
			if(chunk->getTileAt(tileX+1, tileY+1, layer) == tile) state |= SOUTH_EAST;
		}
		if(chunk->getTileAt(tileX+1, tileY, layer) == tile) state |= EAST;
	}
	else if(tileX == CHUNK_BLOCKS-1)
	{
		TerrainChunk *chunkE = &getChunk(chunkX+1, chunkY);
		if(chunkE->getTileAt(0, tileY, layer) == tile) state |= EAST;
		if(tileY == 0)
		{
			if(getChunk(chunkX+1, chunkY-1).getTileAt(0, CHUNK_BLOCKS-1, layer) == tile) state |= NORTH_EAST;
			if(chunkE->getTileAt(0, tileY+1, layer) == tile) state |= SOUTH_EAST;
			if(chunkN->getTileAt(tileX-1, CHUNK_BLOCKS-1, layer) == tile) state |= NORTH_WEST;
			if(chunk->getTileAt(tileX-1, tileY+1, layer) == tile) state |= SOUTH_WEST;
		}
		else if(tileY == CHUNK_BLOCKS-1)
		{
			if(getChunk(chunkX+1, chunkY+1).getTileAt(0, 0, layer) == tile) state |= SOUTH_EAST;
			if(chunkE->getTileAt(0, tileY-1, layer) == tile) state |= NORTH_EAST;
			if(chunk->getTileAt(tileX-1, tileY-1, layer) == tile) state |= NORTH_WEST;
			if(chunkS->getTileAt(tileX-1, 0, layer) == tile) state |= SOUTH_WEST;
		}
		else
		{
			if(chunkE->getTileAt(0, tileY-1, layer) == tile) state |= NORTH_EAST;
			if(chunkE->getTileAt(0, tileY+1, layer) == tile) state |= SOUTH_EAST;
			if(chunk->getTileAt(tileX-1, tileY-1, layer) == tile) state |= NORTH_WEST;
			if(chunk->getTileAt(tileX-1, tileY+1, layer) == tile) state |= SOUTH_WEST;
		}
		if(chunk->getTileAt(tileX-1, tileY, layer) == tile) state |= WEST;
	}
	else
	{
		if(chunk->getTileAt(tileX-1, tileY, layer) == tile) state |= WEST;
		if(chunk->getTileAt(tileX+1, tileY, layer) == tile) state |= EAST;
		if(tileY == 0)
		{
			if(chunkN->getTileAt(tileX+1, CHUNK_BLOCKS-1, layer) == tile) state |= NORTH_EAST;
			if(chunkN->getTileAt(tileX-1, CHUNK_BLOCKS-1, layer) == tile) state |= NORTH_WEST;
			if(chunk->getTileAt(tileX+1, tileY+1, layer) == tile) state |= SOUTH_EAST;
			if(chunk->getTileAt(tileX-1, tileY+1, layer) == tile) state |= SOUTH_WEST;
		}
		else if(tileY == CHUNK_BLOCKS-1)
		{
			if(chunkS->getTileAt(tileX+1, 0, layer) == tile) state |= SOUTH_EAST;
			if(chunkS->getTileAt(tileX-1, 0, layer) == tile) state |= SOUTH_WEST;
			if(chunk->getTileAt(tileX+1, tileY-1, layer) == tile) state |= NORTH_EAST;
			if(chunk->getTileAt(tileX-1, tileY-1, layer) == tile) state |= NORTH_WEST;
		}
		else
		{
			if(chunk->getTileAt(tileX+1, tileY-1, layer) == tile) state |= NORTH_EAST;
			if(chunk->getTileAt(tileX-1, tileY-1, layer) == tile) state |= NORTH_WEST;
			if(chunk->getTileAt(tileX+1, tileY+1, layer) == tile) state |= SOUTH_EAST;
			if(chunk->getTileAt(tileX-1, tileY+1, layer) == tile) state |= SOUTH_WEST;
		}
	}
		
	return state;
}
	
// TILE MODIFICATION
bool Terrain::setTile(const int x, const int y, BlockID tile, const TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	if(getChunk(XMath::floor(x / CHUNK_PXF), XMath::floor(y / CHUNK_PXF)).setTile(XMath::mod(x, CHUNK_PX), XMath::mod(y, CHUNK_PX), tile, layer))
	{	
		// Update neighbouring tiles
		getChunk(XMath::floor(x     / CHUNK_PXF), XMath::floor(y     / CHUNK_PXF)).updateTile(XMath::mod(x,   CHUNK_PX), XMath::mod(y,   CHUNK_PX), getTileState(x,   y), true);
		getChunk(XMath::floor((x+1) / CHUNK_PXF), XMath::floor(y     / CHUNK_PXF)).updateTile(XMath::mod(x+1, CHUNK_PX), XMath::mod(y,   CHUNK_PX), getTileState(x+1, y), true);
		getChunk(XMath::floor((x-1) / CHUNK_PXF), XMath::floor(y     / CHUNK_PXF)).updateTile(XMath::mod(x-1, CHUNK_PX), XMath::mod(y,   CHUNK_PX), getTileState(x-1, y), true);
		getChunk(XMath::floor(x     / CHUNK_PXF), XMath::floor((y+1) / CHUNK_PXF)).updateTile(XMath::mod(x,   CHUNK_PX), XMath::mod(y+1, CHUNK_PX), getTileState(x, y+1), true);
		getChunk(XMath::floor(x     / CHUNK_PXF), XMath::floor((y-1) / CHUNK_PXF)).updateTile(XMath::mod(x,   CHUNK_PX), XMath::mod(y-1, CHUNK_PX), getTileState(x, y-1), true);
			
		getChunk(XMath::floor((x+1) / CHUNK_PXF), XMath::floor((y+1) / CHUNK_PXF)).updateTile(XMath::mod(x+1, CHUNK_PX), XMath::mod(y+1, CHUNK_PX), getTileState(x+1, y+1));
		getChunk(XMath::floor((x-1) / CHUNK_PXF), XMath::floor((y+1) / CHUNK_PXF)).updateTile(XMath::mod(x-1, CHUNK_PX), XMath::mod(y+1, CHUNK_PX), getTileState(x-1, y+1));
		getChunk(XMath::floor((x-1) / CHUNK_PXF), XMath::floor((y-1) / CHUNK_PXF)).updateTile(XMath::mod(x-1, CHUNK_PX), XMath::mod(y-1, CHUNK_PX), getTileState(x-1, y-1));
		getChunk(XMath::floor((x+1) / CHUNK_PXF), XMath::floor((y-1) / CHUNK_PXF)).updateTile(XMath::mod(x+1, CHUNK_PX), XMath::mod(y-1, CHUNK_PX), getTileState(x+1, y-1));
			
		return true;
	}
	return false;
}
	
bool Terrain::removeTile(const int x, const int y, TerrainLayer layer = TERRAIN_LAYER_SCENE)
{
	if(getChunk(XMath::floor(x / CHUNK_PXF), XMath::floor(y / CHUNK_PXF)).setTile(XMath::mod(x, CHUNK_PX), XMath::mod(y, CHUNK_PX), BLOCK_EMPTY, layer))
	{
		// Update neighbouring tiles
		getChunk(XMath::floor(x     / CHUNK_PXF), XMath::floor(y     / CHUNK_PXF)).updateTile(XMath::mod(x,   CHUNK_PX), XMath::mod(y,   CHUNK_PX), getTileState(x,   y), true);
		getChunk(XMath::floor((x+1) / CHUNK_PXF), XMath::floor(y     / CHUNK_PXF)).updateTile(XMath::mod(x+1, CHUNK_PX), XMath::mod(y,   CHUNK_PX), getTileState(x+1, y), true);
		getChunk(XMath::floor((x-1) / CHUNK_PXF), XMath::floor(y     / CHUNK_PXF)).updateTile(XMath::mod(x-1, CHUNK_PX), XMath::mod(y,   CHUNK_PX), getTileState(x-1, y), true);
		getChunk(XMath::floor(x     / CHUNK_PXF), XMath::floor((y+1) / CHUNK_PXF)).updateTile(XMath::mod(x,   CHUNK_PX), XMath::mod(y+1, CHUNK_PX), getTileState(x, y+1), true);
		getChunk(XMath::floor(x     / CHUNK_PXF), XMath::floor((y-1) / CHUNK_PXF)).updateTile(XMath::mod(x,   CHUNK_PX), XMath::mod(y-1, CHUNK_PX), getTileState(x, y-1), true);
			
		getChunk(XMath::floor((x+1) / CHUNK_PXF), XMath::floor((y+1) / CHUNK_PXF)).updateTile(XMath::mod(x+1, CHUNK_PX), XMath::mod(y+1, CHUNK_PX), getTileState(x+1, y+1));
		getChunk(XMath::floor((x-1) / CHUNK_PXF), XMath::floor((y+1) / CHUNK_PXF)).updateTile(XMath::mod(x-1, CHUNK_PX), XMath::mod(y+1, CHUNK_PX), getTileState(x-1, y+1));
		getChunk(XMath::floor((x-1) / CHUNK_PXF), XMath::floor((y-1) / CHUNK_PXF)).updateTile(XMath::mod(x-1, CHUNK_PX), XMath::mod(y-1, CHUNK_PX), getTileState(x-1, y-1));
		getChunk(XMath::floor((x+1) / CHUNK_PXF), XMath::floor((y-1) / CHUNK_PXF)).updateTile(XMath::mod(x+1, CHUNK_PX), XMath::mod(y-1, CHUNK_PX), getTileState(x+1, y-1));
			
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
				chunkLoadQueue.push_front(chunk); // Add to load queue
			}
				
			chunks[key] = chunk;
			return *chunk;
		}
		return m_dummyChunk; // Create dummy
	}
	return *chunks[key];
}
	
void Terrain::loadVisibleChunks()
{
	int x0 = XMath::floor(Camera::getPosition().x/CHUNK_PXF);
	int y0 = XMath::floor(Camera::getPosition().y/CHUNK_PXF);
	int x1 = XMath::floor((Camera::getPosition().x+XWindow::getSize().x)/CHUNK_PXF);
	int y1 = XMath::floor((Camera::getPosition().y+XWindow::getSize().y)/CHUNK_PXF);
		
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
	int cx = XMath::floor(Camera::getCenter().x/CHUNK_PXF);
	int cy = XMath::floor(Camera::getCenter().y/CHUNK_PXF);
	TerrainChunk *chunk = 0;
	if((chunk = &getChunk(cx, cy, true))->getState() != CHUNK_INITIALIZED)
	{
		LOG("Insta-generate chunk [%i, %i]", cx, cy);
		chunkLoadQueue.remove(chunk);
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
	int x0 = XMath::floor(Camera::getPosition().x/CHUNK_PXF);
	int y0 = XMath::floor(Camera::getPosition().y/CHUNK_PXF);
	int x1 = XMath::floor((Camera::getPosition().x+XWindow::getSize().x)/CHUNK_PXF);
	int y1 = XMath::floor((Camera::getPosition().y+XWindow::getSize().y)/CHUNK_PXF);
		
	for(int y = y0; y <= y1; y++)
	{
		for(int x = x0; x <= x1; x++)
		{
			batch->pushMatrix(Matrix4().translate(x * CHUNK_PXF, y * CHUNK_PXF, 0.0f));
			getChunk(x, y, true).draw(batch);
			batch->popMatrix();
		}
	}
}