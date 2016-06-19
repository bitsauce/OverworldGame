#include "Lighting.h"
#include "Constants.h"
#include "World/World.h"

Lighting::Lighting(World *world) :
	m_world(world),
	m_enabled(true),
	m_redraw(false)
{
}

Lighting::~Lighting()
{
}

void Lighting::addLightSource(LightSource *source)
{
	m_lightSources.push_back(source);
	m_redraw = true;
}

void Lighting::onDraw(DrawEvent *e)
{
	if(!m_enabled) return;

	ChunkManager *chunkManager = m_world->getTerrain()->getChunkManager();
	const float width = chunkManager->m_loadingArea.getWidth() * CHUNK_BLOCKSF, height = chunkManager->m_loadingArea.getHeight() * CHUNK_BLOCKSF;

	GraphicsContext *context = e->getGraphicsContext();

	context->setShader(0);
	context->disable(GraphicsContext::BLEND);
	context->setRenderTarget(chunkManager->m_lightingPass2);
	context->setTexture(chunkManager->m_staticLightingRenderTarget->getTexture());
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Draw static light sources
	context->enable(GraphicsContext::BLEND);
	context->setShader(chunkManager->m_radialLightingShader);
	context->setBlendState(BlendState::PRESET_ADDITIVE);

	// Loop through and draw light sources to global render-target
	for(LightSource *light : m_lightSources)
	{
		if(light->getType() != LightSource::DYNAMIC) continue;

		chunkManager->m_radialLightingShader->setUniform2f("u_Radius", light->getRadius() / width, light->getRadius() / height);
		chunkManager->m_radialLightingShader->setUniform3f("u_Color", light->getColor().getR() / 255.0f, light->getColor().getG() / 255.0f, light->getColor().getB() / 255.0f);

		int xc = math::floor(light->getPosition().x / width), yc = math::floor(light->getPosition().y / height),
			x0 = math::floor((light->getPosition().x - light->getRadius()) / width), y0 = math::floor((light->getPosition().y - light->getRadius()) / height),
			x1 = math::floor((light->getPosition().x + light->getRadius()) / width), y1 = math::floor((light->getPosition().y + light->getRadius()) / height);
		for(int y = y0; y <= y1; y++)
		{
			for(int x = x0; x <= x1; x++)
			{
				// Draw light when on border
				const Vector2F lightPos = light->getPosition(); float intpart;
				const Vector2F drawPos = Vector2F(math::mod(lightPos.x, width) + (xc - x) * width + modf(lightPos.x, &intpart), math::mod(lightPos.y, height) + (yc - y) * height + modf(lightPos.y, &intpart));
				chunkManager->m_radialLightingShader->setUniform2f("u_LightTexCoord", drawPos.x / width, drawPos.y / height);
				context->drawCircle(drawPos, light->getRadius(), light->getRadius() * 1.5f);
			}
		}
	}

	context->disable(GraphicsContext::BLEND);

	// Blur horizontally (pass 1)
	context->setRenderTarget(chunkManager->m_lightingPass0);
	context->setShader(chunkManager->m_blurHShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Blur vertically (pass 2)
	context->setRenderTarget(chunkManager->m_lightingPass1);
	context->setShader(chunkManager->m_blurVShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	context->setShader(0);
	context->setRenderTarget(0);

	context->enable(GraphicsContext::BLEND);
	
	ChunkManager::ChunkArea area = m_world->getTerrain()->getChunkManager()->getLoadingArea();

	// Re-enable alpha blending
	context->setBlendState(BlendState::PRESET_MULTIPLY);

	context->setTransformationMatrix(m_world->getCamera()->getTransformationMatrix(e->getAlpha()));
	context->setTexture(m_world->getTerrain()->getChunkManager()->m_lightingPass1->getTexture());

	float u0 = area.x0 / (float) area.getWidth(),
		v0 = area.y0 / (float) area.getHeight(),
		u1 = u0 + 1.0f,
		v1 = v0 + 1.0f;

	context->drawRectangle(area.x0 * CHUNK_PXF, area.y0 * CHUNK_PXF, area.getWidth() * CHUNK_PXF, area.getHeight() * CHUNK_PXF, Color(255), TextureRegion(u0, v0, u1, v1));
	context->setBlendState(BlendState::PRESET_ALPHA_BLEND);
}