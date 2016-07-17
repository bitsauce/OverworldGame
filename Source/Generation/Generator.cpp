#include "Constants.h"
#include "Generator.h"
//#include "Grassland.h"
#include "Game/RayCast.h"

//#include "Structure.h"
//#include "Structures/OakTree.h"

ChunkGenerator::ChunkGenerator(const uint seed) :
	m_seed(seed),
	m_graphicsContext(Game::GetInstance()->getWindow()->getGraphicsContext())
{
	m_random.setSeed(seed);

	m_generationShader = Game::GetInstance()->getResourceManager()->get<Shader>("Shaders/Generation");
	m_generationShader->setUniform1i("u_ShowNoise", false);
	m_generationShader->setUniform2f("u_Resolution", CHUNK_BLOCKS, CHUNK_BLOCKS);
	m_generationShader->setUniform1ui("u_Seed", seed);
	m_generationShader->setUniform1f("u_CliffingDelta", 70.0f);

	m_renderTarget = Resource<RenderTarget2D>(new RenderTarget2D(CHUNK_BLOCKS, CHUNK_BLOCKS));

	m_blockData[BLOCK_EMPTY] = BlockData::get("Empty_Block");
	m_blockData[BLOCK_GRASS] = BlockData::get("Grass_Block");
	m_blockData[BLOCK_DIRT_BACK] = BlockData::get("Dirt_Backdrop");
	m_blockData[BLOCK_STONE] = BlockData::get("Stone_Block");
}

void ChunkGenerator::getChunkBlocks(const int chunkX, const int chunkY, ChunkBlock *blocks)
{
	// Load structures
	//loadStructures(chunkX, chunkY);

	// Load structures?
	// TODO: Not sure this should be here, but it cant be in WorldGenerator::getChunkBlocks either because
	// the chunk has to be loaded when we know that all the structures that will affect this chunk are generated.
	//BlockID *structureBlocks = m_chunkStructures[CHUNK_KEY(chunkX, chunkY)];

	m_generationShader->setUniform2f("u_Position", chunkX * CHUNK_BLOCKS, chunkY * CHUNK_BLOCKS);
	m_graphicsContext->setRenderTarget(m_renderTarget.get());
	//m_graphicsContext->disable(GraphicsContext::BLEND);
	m_graphicsContext->setShader(m_generationShader);
	m_graphicsContext->drawRectangle(0, 0, CHUNK_BLOCKSF, CHUNK_BLOCKSF);
	m_graphicsContext->setShader(0);
	m_graphicsContext->setRenderTarget(0);
	//m_graphicsContext->enable(GraphicsContext::BLEND);

	Pixmap pixmap = m_renderTarget->getTexture()->getPixmap();
	uchar pixel[4];
	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			pixmap.getPixel(x, y, pixel);
			for(int z = 0; z < WORLD_LAYER_COUNT; ++z)
			{
				blocks[BLOCK_INDEX(x, y, z)].setBlockData(BlockData::get(pixel[z]));
			}
		}
	}

	// Load blocks
	/*const int tileX = chunkX * CHUNK_BLOCKS;
	const int tileY = chunkY * CHUNK_BLOCKS;
	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(int z = 0; z < WORLD_LAYER_COUNT; ++z)
			{
				//BlockID structureBlock;
				//if((structureBlock = structureBlocks[BLOCK_INDEX(x, y, z)]) == BLOCK_EMPTY)
				{
					blocks[BLOCK_INDEX(x, y, z)].setBlockData(m_blockData[getGroundAt(tileX + x, tileY + y, (WorldLayer) z)]);
				}
				//else
				{
				//	blocks[BLOCK_INDEX(x, y, z)] = structureBlock;
				}
			}
		}
	}*/
}
	
ChunkGenerator::BlockType ChunkGenerator::getGroundAt(const int x, const int y, const WorldLayer layer)
{
	switch(layer)
	{
		case WORLD_LAYER_MIDDLE:
		{
			float h = m_noise.valueAt(x * 0.1f, y + 710239) * 7;

			// Ground
			if((math::clamp((32 - y) / 32.0f, 0.0f, 1.0f) + (m_noise.valueAt(x, y) * 0.5f + 0.5f)) * math::step(0.0f, y + h) > 0.5f)
			{
				float f = m_noise.valueAt(x + 2989, y + 7238) * 0.5f + 0.5f;

				f *= (math::clamp(y / 1000.0f, 0.0f, 1.0f));

				return (f < 0.1f) ? BLOCK_GRASS : BLOCK_STONE;
			}
		}
		break;
			
		case WORLD_LAYER_BACK:
		{
			if(y > 20)
			{
				return BLOCK_DIRT_BACK;
			}
		}
		break;
	}
	return BLOCK_EMPTY;
}

void ChunkGenerator::loadStructures(const int chunkX, const int chunkY)
{
	// Create list
	/*list<Structure*> structures;
	if(m_chunkStructures.find(CHUNK_KEY(chunkX, chunkY)) == m_chunkStructures.end())
	{
		BlockID *blocks = new BlockID[CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT];
		for(int i = 0; i < CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT; ++i)
		{
			blocks[i] = BLOCK_EMPTY;
		}
		m_chunkStructures[CHUNK_KEY(chunkX, chunkY)] = blocks;
	}

	// Load structures
	for(int x = 0; x < CHUNK_BLOCKS; ++x)
	{
		int tileX = CHUNK_BLOCKS * chunkX + x;
		if(m_random.getDouble(tileX + m_seed) < 0.025/*TREE_CHANCE*//*)
		{
			structures.push_back(new OakTree(tileX, getGroundHeight(tileX)));
		}
	}

	// Place structures
	for(Structure *structure : structures)
	{
		structure->place(this);
		delete structure;
	}*/
}

void ChunkGenerator::setBlockAt(const int x, const int y, const WorldLayer z, const BlockID block)
{
	int chunkX = (int) floor(x / CHUNK_BLOCKSF),
		chunkY = (int) floor(y / CHUNK_BLOCKSF);
	if(m_chunkStructures.find(CHUNK_KEY(chunkX, chunkY)) == m_chunkStructures.end())
	{
		BlockID *blocks = new BlockID[CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT];
		for(int i = 0; i < CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT; ++i)
		{
			blocks[i] = 0;
		}
		m_chunkStructures[CHUNK_KEY(chunkX, chunkY)] = blocks;
	}
	m_chunkStructures[CHUNK_KEY(chunkX, chunkY)][BLOCK_INDEX(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), z)] = block;
}

int ChunkGenerator::getGroundHeight(const int x)
{
	int y = 0;
	if(getGroundAt(x, y, WORLD_LAYER_MIDDLE) == 0)
	{
		while(getGroundAt(x, ++y, WORLD_LAYER_MIDDLE) == 0); y--;
	}
	else
	{
		while(getGroundAt(x, --y, WORLD_LAYER_MIDDLE) != 0);
	}
	return y;
}
	
bool isFlatStretch(int start, int size)
{
	/*int height = getGroundHeight(start);
	for(int x = start+1; x < start+size; x++)
	{
		if(getGroundHeight(x) != height)
			return false;
	}*/
	return true;
}