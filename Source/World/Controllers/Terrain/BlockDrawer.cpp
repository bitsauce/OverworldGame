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
	
	// If world layer == middle we draw blocks to a render target instead (used for shadow casting)
	if(m_layer == WORLD_LAYER_MIDDLE)
	{
		graphicsContext->disable(GraphicsContext::BLEND);
		graphicsContext->setRenderTarget(m_chunkManager->m_shadowCasterRenderTarget);
	}

	graphicsContext->setBlendState(BlendState(BlendState::BLEND_ONE, BlendState::BLEND_ONE_MINUS_SRC_ALPHA));
	graphicsContext->setShader(m_chunkManager->m_tileMapShader);
	m_chunkManager->m_tileMapShader->setSampler2D("u_SortedBlockTexture", m_chunkManager->m_sortedBlocksRenderTarget[m_layer]->getTexture(0));
	m_chunkManager->m_tileMapShader->setSampler2D("u_SortedQuadTexture", m_chunkManager->m_sortedBlocksRenderTarget[m_layer]->getTexture(1));

	if(m_layer == WORLD_LAYER_MIDDLE)
	{
		Vector2F cameraPosition = m_camera->getDrawPosition(e->getAlpha()), cameraSize = m_camera->getSize();
		float u0 = cameraPosition.x / (area.getWidth() * CHUNK_PXF),
			v0 = cameraPosition.y / (area.getHeight() * CHUNK_PXF),
			u1 = (cameraPosition.x + cameraSize.x) / (area.getWidth() * CHUNK_PXF),
			v1 = (cameraPosition.y + cameraSize.y) / (area.getHeight() * CHUNK_PXF);

		graphicsContext->drawRectangle(0, 0, m_chunkManager->m_shadowCasterRenderTarget->getWidth(), m_chunkManager->m_shadowCasterRenderTarget->getHeight(), Color(255), TextureRegion(u0, v0, u1, v1));
	}
	else
	{
		float u0 = area.x0 / (float) area.getWidth(),
			v0 = area.y0 / (float) area.getHeight(),
			u1 = u0 + 1.0f,
			v1 = v0 + 1.0f;
		graphicsContext->drawRectangle(area.x0 * CHUNK_PXF, area.y0 * CHUNK_PXF, area.getWidth() * CHUNK_PXF, area.getHeight() * CHUNK_PXF, Color(255), TextureRegion(u0, v0, u1, v1));
	}
	graphicsContext->setShader(0);
	graphicsContext->setBlendState(BlendState(BlendState::PRESET_ALPHA_BLEND));

	// TODO: This should probably not be here
	if(m_layer == WORLD_LAYER_MIDDLE)
	{
		graphicsContext->enable(GraphicsContext::BLEND);
		graphicsContext->setRenderTarget(0);
		graphicsContext->setTexture(m_chunkManager->m_shadowCasterRenderTarget->getTexture());
		graphicsContext->drawRectangle(0, 0, m_chunkManager->m_shadowCasterRenderTarget->getWidth(), m_chunkManager->m_shadowCasterRenderTarget->getHeight());
		graphicsContext->setTexture(0);

		graphicsContext->setShader(m_chunkManager->m_blockEntityShader);

		// Draw block entitites
		ChunkManager::ChunkArea activeArea = m_chunkManager->getActiveArea();
		for(int y = activeArea.y0; y <= activeArea.y1; ++y)
		{
			for(int x = activeArea.x0; x <= activeArea.x1; ++x)
			{
				Chunk *chunk = m_chunkManager->getChunkAt(x, y, true);
				m_chunkManager->m_blockEntityShader->setSampler2D("u_TimeOffsets", chunk->m_timeOffsetTexture);
				chunk->drawBlockEntities(graphicsContext);
			}
		}

		graphicsContext->setShader(0);
	}
}