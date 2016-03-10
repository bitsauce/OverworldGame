#include "BlockDrawer.h"
#include "World/World.h"
#include "Constants.h"

BlockDrawer::BlockDrawer(World *world, const WorldLayer layer) :
	m_chunkManager(world->getTerrain()->getChunkManager()),
	m_camera(world->getCamera()),
	m_layer(layer)
{
}

void BlockDrawer::onDraw(DrawEvent *e)
{
	// Get sprite batch
	GraphicsContext *graphicsContext = e->getGraphicsContext();
	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();

	// Flush to set the draw order straight
	spriteBatch->flush();

	// Setup graphics context
	graphicsContext->setTransformationMatrix(m_camera->getTransformationMatrix(e->getAlpha()));

	// Draw chunk area
	ChunkManager::ChunkArea area = m_chunkManager->getLoadingArea();

	graphicsContext->setBlendState(BlendState(BlendState::BLEND_ONE, BlendState::BLEND_ONE_MINUS_SRC_ALPHA));
	graphicsContext->setShader(m_chunkManager->m_tileMapShader);
	m_chunkManager->m_tileMapShader->setSampler2D("u_SortedBlockTexture", m_chunkManager->m_sortedBlocksRenderTarget[m_layer]->getTexture(0));
	m_chunkManager->m_tileMapShader->setSampler2D("u_SortedQuadTexture", m_chunkManager->m_sortedBlocksRenderTarget[m_layer]->getTexture(1));

	float u0 = area.x0 / (float) area.getWidth(),
		v0 = area.y0 / (float) area.getHeight(),
		u1 = u0 + 1.0f,
		v1 = v0 + 1.0f;

	graphicsContext->drawRectangle(area.x0 * CHUNK_PXF, area.y0 * CHUNK_PXF, area.getWidth() * CHUNK_PXF, area.getHeight() * CHUNK_PXF, Color(255), TextureRegion(u0, v0, u1, v1));
	graphicsContext->setShader(0);
	graphicsContext->setBlendState(BlendState(BlendState::PRESET_ALPHA_BLEND));
}