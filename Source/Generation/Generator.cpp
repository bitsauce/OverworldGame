#include "Constants.h"
#include "Generator.h"
//#include "Grassland.h"
#include "Game/RayCast.h"

#include "Formations/OakForest.h"

const float CLIFFING_DELTA = 70.0f;

ChunkGenerator::ChunkGenerator(const uint seed) :
	m_seed(seed),
	m_graphicsContext(Game::Get()->getWindow()->getGraphicsContext())
{
	//m_random.setSeed(seed);

	m_generationShader = Resource<Shader>("Shaders/Generation");
	m_generationShader->setUniform1i("u_ShowNoise", false);
	m_generationShader->setUniform2f("u_Resolution", CHUNK_BLOCKS, CHUNK_BLOCKS);
	m_generationShader->setUniform1ui("u_Seed", seed % 1000); // NOTE TO SELF: This is not the right way - here we can only get 1000 different worlds.
															  // u_Seed should change the hash function in the shader instead
	m_generationShader->setUniform1f("u_CliffingDelta", CLIFFING_DELTA);

	m_groundHeightShader = Resource<Shader>("Shaders/Generation/GroundHeight");
	m_groundHeightShader->setUniform1ui("u_Seed", seed % 1000);
	m_groundHeightShader->setUniform1f("u_ResolutionX", CHUNK_BLOCKS);
	m_groundHeightShader->setUniform1f("u_CliffingDelta", CLIFFING_DELTA);

	m_renderTarget = new RenderTarget2D(CHUNK_BLOCKS, CHUNK_BLOCKS);
	m_groundHeightRenderTarget = new RenderTarget2D(CHUNK_BLOCKS, 1, 1, PixelFormat(PixelFormat::R, PixelFormat::INT));

	m_blockData[BLOCK_EMPTY] = BlockData::get("Empty_Block");
	m_blockData[BLOCK_GRASS] = BlockData::get("Grass_Block");
	m_blockData[BLOCK_DIRT_BACK] = BlockData::get("Dirt_Backdrop");
	m_blockData[BLOCK_STONE] = BlockData::get("Stone_Block");

	// Add formations to list
	m_formations.push_back(new OakForest(this));
}

void ChunkGenerator::getBlocks(const int chunkX, const int chunkY, BlockID *blocks)
{
	// Check if ground height is not generated for this chunk
	{
		unordered_map<int, int*>::const_iterator itr = m_groundHeight.find(chunkX);
		if(itr == m_groundHeight.end())
		{
			generateGroundHeight(chunkX);
		}
	}

	// Generate blocks for this chunk using GPU
	m_graphicsContext->pushState();

	// Setup graphics context
	m_graphicsContext->setRenderTarget(m_renderTarget);
	m_graphicsContext->disable(GraphicsContext::BLEND);

	// Set generation offset
	m_generationShader->setUniform2f("u_Position", chunkX * CHUNK_BLOCKS, chunkY * CHUNK_BLOCKS);
	m_generationShader->setUniform1iv("u_GroundHeight", 32, m_groundHeight[chunkX]);
	m_graphicsContext->setShader(m_generationShader);

	// Draw generated blocks to render target
	m_graphicsContext->drawRectangle(0, 0, CHUNK_BLOCKS, CHUNK_BLOCKS);

	// Write result (render target pixel values) to chunk blocks
	Pixmap pixmap = m_renderTarget->getTexture()->getPixmap();
	uchar pixel[4];
	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			pixmap.getPixel(x, y, pixel);
			for(int z = 0; z < WORLD_LAYER_COUNT; ++z)
			{
				blocks[BLOCK_INDEX(x, y, z)] = pixel[z];
			}
		}
	}

	m_graphicsContext->popState();

	// Iterate all types of formations
	for(Formation *formation : m_formations)
	{
		// Get formation relative coordinates for this chunk
		const int formX = int(math::floor(float(chunkX) / formation->getWidth()));
		const int formY = int(math::floor(float(chunkY) / formation->getHeight()));

		// If the formation at this position is not generated
		if(!formation->isGeneratedAt(formX, formY))
		{
			// Generate it
			formation->generate(formX, formY);
		}

		// Get formation blocks for this chunk
		formation->getBlocks(chunkX, chunkY, blocks);
	}
}

int ChunkGenerator::getGroundHeight(const int x)
{
	const int chunkX = math::floor(x / CHUNK_BLOCKSF);
	unordered_map<int, int*>::const_iterator itr = m_groundHeight.find(chunkX);
	if(itr == m_groundHeight.end())
	{
		generateGroundHeight(chunkX);
	}
	return m_groundHeight[chunkX][math::mod(x, CHUNK_BLOCKS)];
}

void ChunkGenerator::generateGroundHeight(const int chunkX)
{
	// Generate ground height for this chunk using GPU
	m_graphicsContext->pushState();

	// Setup graphics context
	m_graphicsContext->setRenderTarget(m_groundHeightRenderTarget);
	m_graphicsContext->disable(GraphicsContext::BLEND);

	// Set generation offset
	m_groundHeightShader->setUniform1f("u_PositionX", chunkX * CHUNK_BLOCKS);
	m_graphicsContext->setShader(m_groundHeightShader);

	// Draw ground height values to render target
	m_graphicsContext->drawRectangle(0, 0, CHUNK_BLOCKS, 1);

	// Write ground height to map
	int *groundHeight = new int[CHUNK_BLOCKS];
	Pixmap pixmap = m_groundHeightRenderTarget->getTexture()->getPixmap();
	int value;
	for(int x = 0; x < CHUNK_BLOCKS; ++x)
	{
		pixmap.getPixel(x, 0, &value);
		groundHeight[x] = value;
	}
	m_groundHeight[chunkX] = groundHeight;

	m_graphicsContext->popState();
}

