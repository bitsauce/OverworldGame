#include "Constants.h"
#include "Generator.h"
//#include "Grassland.h"
#include "Game/RayCast.h"

//#include "Structure.h"
//#include "Structures/OakTree.h"

ChunkGenerator::ChunkGenerator(const uint seed) :
	m_seed(seed),
	m_graphicsContext(Game::Get()->getWindow()->getGraphicsContext())
{
	//m_random.setSeed(seed);

	m_generationShader = Resource<Shader>("Shaders/Generation");
	m_generationShader->setUniform1i("u_ShowNoise", false);
	m_generationShader->setUniform2f("u_Resolution", CHUNK_BLOCKS, CHUNK_BLOCKS);
	m_generationShader->setUniform1ui("u_Seed", seed % 1000);
	m_generationShader->setUniform1f("u_CliffingDelta", 70.0f);

	m_renderTarget = shared_ptr<RenderTarget2D>(new RenderTarget2D(CHUNK_BLOCKS, CHUNK_BLOCKS));

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

	bool wasEnabled = m_graphicsContext->isEnabled(GraphicsContext::BLEND);
	if(wasEnabled)
	{
		m_graphicsContext->disable(GraphicsContext::BLEND);
	}

	m_graphicsContext->setShader(m_generationShader);
	m_graphicsContext->drawRectangle(0, 0, CHUNK_BLOCKSF, CHUNK_BLOCKSF);
	m_graphicsContext->setShader(0);
	m_graphicsContext->setRenderTarget(0);

	if(wasEnabled)
	{
		m_graphicsContext->enable(GraphicsContext::BLEND);
	}

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
}