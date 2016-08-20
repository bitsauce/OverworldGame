#include "Lighting.h"
#include "Constants.h"
#include "World/World.h"

Lighting::Lighting(World *world) :
	m_world(world),
	m_enabled(true),
	m_redrawStaticLighting(false),
	m_lightingPass0(0),
	m_lightingPass1(0),
	m_lightingPass2(0),
	m_occludersRenderTarget(0),
	m_shadowMapRenderTarget(0),
	m_shadowsRenderTarget(0),
	m_blockLightingRenderTarget(0),
	m_directionalLightingShader(Resource<Shader>("Shaders/DirectionalLighting")),
	m_radialLightingShader(Resource<Shader>("Shaders/RadialLighting")),
	m_blurHShader(Resource<Shader>("Shaders/BlurH")),
	m_blurVShader(Resource<Shader>("Shaders/BlurV"))
{
	m_shadowMapShader = Resource<Shader>("Shaders/Shadow_Map");
	m_shadowRenderShader = Resource<Shader>("Shaders/Shadow_Render");

	setLightMapResolution(512);
}

Lighting::~Lighting()
{
}

void Lighting::onDraw(DrawEvent *e)
{
	if(!m_enabled) return;

	if(m_redrawStaticLighting)
	{
		redrawStaticLighting(e->getGraphicsContext());
		m_redrawStaticLighting = false;
	}

}

void Lighting::drawLight(LightSource *light, RenderTarget2D *dest, GraphicsContext *context)
{
}

void Lighting::setLightMapResolution(const int size)
{
	Vector2I viewSize = m_world->getCamera()->getSize();

	delete m_occludersRenderTarget;
	delete m_shadowMapRenderTarget;
	delete m_shadowsRenderTarget;

	m_occludersRenderTarget = new RenderTarget2D(size, size);
	m_occludersRenderTarget->getTexture()->setFiltering(Texture2D::LINEAR);
	m_shadowMapRenderTarget = new RenderTarget2D(size, size);
	m_shadowMapRenderTarget->getTexture()->setFiltering(Texture2D::LINEAR);
	m_shadowsRenderTarget = new RenderTarget2D(viewSize.x, viewSize.y);
	m_shadowMapRenderTarget->getTexture()->setWrapping(Texture2D::REPEAT);

	m_lightMapSize = size;
}

void Lighting::updateViewSize(int width, int height)
{
	// Get load area size
	ChunkManager *chunkManager = m_world->getTerrain()->getChunkManager();
	ChunkManager::ChunkArea loadingArea = chunkManager->getLoadingArea();
	int loadAreaWidth = loadingArea.getWidth();
	int loadAreaHeight = loadingArea.getHeight();

	delete m_lightingPass0;
	delete m_lightingPass1;
	delete m_lightingPass2;
	delete m_blockLightingRenderTarget;

	m_lightingPass0 = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_lightingPass0->getTexture()->setFiltering(Texture2D::LINEAR);
	m_lightingPass0->getTexture()->setWrapping(Texture2D::REPEAT);
	m_lightingPass1 = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_lightingPass1->getTexture()->setFiltering(Texture2D::LINEAR);
	m_lightingPass1->getTexture()->setWrapping(Texture2D::REPEAT);
	m_lightingPass2 = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_lightingPass2->getTexture()->setFiltering(Texture2D::LINEAR);
	m_lightingPass2->getTexture()->setWrapping(Texture2D::REPEAT);

	// Create shadow textures
	m_blockLightingRenderTarget = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_blockLightingRenderTarget->getTexture()->setFiltering(Texture2D::LINEAR);
	m_blockLightingRenderTarget->getTexture()->setWrapping(Texture2D::REPEAT);

	m_directionalLightingShader->setSampler2D("u_Texture", chunkManager->m_blocksRenderTarget->getTexture());
	m_directionalLightingShader->setUniform1f("u_Width", loadAreaWidth * CHUNK_BLOCKS);
	m_directionalLightingShader->setUniform1f("u_Height", loadAreaHeight * CHUNK_BLOCKS);

	m_radialLightingShader->setSampler2D("u_LightMap", chunkManager->m_blocksRenderTarget->getTexture());
	m_radialLightingShader->setUniform1i("u_Iterations", 100);

	//lighting->m_pointlightDynamicShader->setSampler2D("u_BlockTexture", m_blocksRenderTarget->getTexture());

	m_blurHShader->setSampler2D("u_Texture", m_lightingPass0->getTexture());
	m_blurHShader->setUniform1i("u_Width", loadAreaWidth * CHUNK_BLOCKS);

	m_blurVShader->setSampler2D("u_Texture", m_lightingPass1->getTexture());
	m_blurVShader->setUniform1i("u_Height", loadAreaHeight * CHUNK_BLOCKS);

}

void Lighting::redrawStaticLighting(GraphicsContext *context)
{
	ChunkManager::ChunkArea loadingArea = m_world->getTerrain()->getChunkManager()->getLoadingArea();
	const float width = loadingArea.getWidth() * CHUNK_BLOCKSF, height = loadingArea.getHeight() * CHUNK_BLOCKSF;

	// Directional light // TODO: Directional lighting cannot be here in the static lighting part
	m_directionalLightingShader->setUniform1f("u_OffsetY", (loadingArea.y0 * CHUNK_BLOCKSF - 32.0f) / (loadingArea.getHeight() * CHUNK_BLOCKS));
	m_directionalLightingShader->setUniform1f("u_Direction", 0.0174532925f * 180.0f * (m_world->getTimeOfDay()->isDay() ? (1140.0f - m_world->getTimeOfDay()->getTime()) : (1860.0f - (m_world->getTimeOfDay()->getTime() >= 1140.0f ? m_world->getTimeOfDay()->getTime() : m_world->getTimeOfDay()->getTime() + 1440.0f))) / 720.0f);
	context->setRenderTarget(m_blockLightingRenderTarget);
	context->setShader(m_directionalLightingShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Draw static light sources
	context->enable(GraphicsContext::BLEND);
	context->setShader(m_radialLightingShader);
	context->setBlendState(BlendState::PRESET_ADDITIVE);

	// Loop through and draw light sources to global render-target
	for(LightSource *light : m_lightSources)
	{
		if(light->getType() != LightSource::STATIC) continue;

		m_radialLightingShader->setUniform2f("u_Radius", light->getRadius() / width, light->getRadius() / height);
		m_radialLightingShader->setUniform3f("u_Color", light->getColor().getR() / 255.0f, light->getColor().getG() / 255.0f, light->getColor().getB() / 255.0f);

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
				m_radialLightingShader->setUniform2f("u_LightTexCoord", drawPos.x / width, drawPos.y / height);
				context->drawCircle(drawPos, light->getRadius(), light->getRadius() * 1.5f);
			}
		}
	}

	// Reset graphics context
	context->disable(GraphicsContext::BLEND);
	context->setShader(0);
	context->setRenderTarget(0);
}

void Lighting::addLightSource(LightSource *source)
{
	m_lightSources.push_back(source);
	m_redrawStaticLighting = true;
}

void Lighting::removeLightSource(LightSource *lightSource)
{
	m_lightSources.remove(lightSource);
}