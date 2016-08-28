#include "Lighting.h"
#include "Constants.h"
#include "World/World.h"

Lighting::Lighting(World *world, Window *window) :
	m_world(world),
	m_enabled(true),
	m_redrawStaticLighting(false),
	m_lightingPass0(0),
	m_lightingPass1(0),
	m_lightingPass2(0),
	m_occludersRenderTarget(0),
	m_shadowMapRenderTarget(0),
	m_shadowsRenderTarget(0),
	m_staticLightingRenderTarget(0),
	m_initBlockLightingShader(Resource<Shader>("Shaders/Init_Block_Lighting")),
	m_radialLightingShader(Resource<Shader>("Shaders/RadialLighting")),
	m_blurHShader(Resource<Shader>("Shaders/BlurH")),
	m_blurVShader(Resource<Shader>("Shaders/BlurV"))
{
	m_shadowMapShader = Resource<Shader>("Shaders/Shadow_Map");
	m_shadowRenderShader = Resource<Shader>("Shaders/Shadow_Render");

	setLightMapResolution(512);

	// Update size of textures and such
	updateViewSize(window->getWidth(), window->getHeight());
}

Lighting::~Lighting()
{
}

void Lighting::onDraw(DrawEvent *e)
{
	if(!m_enabled) return;

	GraphicsContext *context = e->getGraphicsContext();

	// Redraw static lighting if need be
	if(m_redrawStaticLighting)
	{
		redrawStaticLighting(context);
		m_redrawStaticLighting = false;
	}

	ChunkManager::ChunkArea loadingArea = m_world->getTerrain()->getChunkManager()->getLoadingArea();
	const float width = loadingArea.getWidth() * CHUNK_BLOCKSF, height = loadingArea.getHeight() * CHUNK_BLOCKSF;

	context->disable(GraphicsContext::BLEND);

	// Clear dynamic lighting
	context->setRenderTarget(m_lightingPass2);
	context->drawRectangle(0.0f, 0.0f, width, height, Color(0, 0, 0, 0));

	// Draw dynamic light sources
	context->enable(GraphicsContext::BLEND);
	context->setBlendState(BlendState::PRESET_ADDITIVE);
	context->setShader(m_radialLightingShader);
	for(LightSource *light : m_lightSources)
	{
		if(light->getType() != LightSource::DYNAMIC) continue;

		m_radialLightingShader->setUniform2f("u_Radius", light->getRadius() / width, light->getRadius() / height);
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
				context->drawCircleGradient(drawPos, light->getRadius(), light->getRadius() * 1.5f, light->getColor(), Color(0, 0, 0, 0));
			}
		}
	}
	context->disable(GraphicsContext::BLEND);

	// Blur horizontally (pass 0)
	m_blurHShader->setSampler2D("u_Texture", m_lightingPass2->getTexture());
	context->setRenderTarget(m_lightingPass0);
	context->setShader(m_blurHShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Blur vertically (pass 1)
	m_blurVShader->setSampler2D("u_Texture", m_lightingPass0->getTexture());
	context->setRenderTarget(m_lightingPass2);
	context->setShader(m_blurVShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Add static lighting to final render target
	context->enable(GraphicsContext::BLEND);
	context->setBlendState(BlendState::PRESET_ADDITIVE);
	context->setShader(0);
	context->setTexture(m_lightingPass1->getTexture());
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Draw final light render target over the screen
	context->setRenderTarget(0);
	context->setBlendState(BlendState::PRESET_MULTIPLY);
	context->setTransformationMatrix(m_world->getCamera()->getTransformationMatrix(e->getAlpha()));
	context->setTexture(m_lightingPass2->getTexture());

	float u0 = loadingArea.x0 / (float) loadingArea.getWidth(),
		v0 = loadingArea.y0 / (float) loadingArea.getHeight(),
		u1 = u0 + 1.0f,
		v1 = v0 + 1.0f;

	context->drawRectangle(loadingArea.x0 * CHUNK_PXF, loadingArea.y0 * CHUNK_PXF, loadingArea.getWidth() * CHUNK_PXF, loadingArea.getHeight() * CHUNK_PXF, Color(255), TextureRegion(u0, v0, u1, v1));
	context->setBlendState(BlendState::PRESET_ALPHA_BLEND);
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

void Lighting::onWindowSizeChanged(WindowEvent *e)
{
	updateViewSize(e->getWidth(), e->getHeight());
}

void Lighting::updateViewSize(int width, int height)
{
	// Get load area size
	ChunkManager *chunkManager = m_world->getTerrain()->getChunkManager();
	int loadAreaWidth = (int) (floor(width * 0.5f / CHUNK_PXF) * 2 + 3) + chunkManager->getLoadAreaRadius() * 2;
	int loadAreaHeight = (int) (floor(height * 0.5f / CHUNK_PXF) * 2 + 3) + chunkManager->getLoadAreaRadius() * 2;

	delete m_lightingPass0;
	delete m_lightingPass1;
	delete m_lightingPass2;
	delete m_staticLightingRenderTarget;

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
	m_staticLightingRenderTarget = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_staticLightingRenderTarget->getTexture()->setFiltering(Texture2D::LINEAR);
	m_staticLightingRenderTarget->getTexture()->setWrapping(Texture2D::REPEAT);

	m_initBlockLightingShader->setSampler2D("u_Texture", chunkManager->m_blocksRenderTarget->getTexture());

	m_radialLightingShader->setSampler2D("u_LightMap", chunkManager->m_blocksRenderTarget->getTexture());
	m_radialLightingShader->setUniform1i("u_Iterations", 32);

	//lighting->m_pointlightDynamicShader->setSampler2D("u_BlockTexture", m_blocksRenderTarget->getTexture());

	//m_blurHShader->setSampler2D("u_Texture", m_staticLightingRenderTarget->getTexture());
	m_blurHShader->setUniform1i("u_Width", loadAreaWidth * CHUNK_BLOCKS);

	//m_blurVShader->setSampler2D("u_Texture", m_lightingPass0->getTexture());
	m_blurVShader->setUniform1i("u_Height", loadAreaHeight * CHUNK_BLOCKS);
}

void Lighting::redrawStaticLighting(GraphicsContext *context)
{
	ChunkManager::ChunkArea loadingArea = m_world->getTerrain()->getChunkManager()->getLoadingArea();
	const float width = loadingArea.getWidth() * CHUNK_BLOCKSF, height = loadingArea.getHeight() * CHUNK_BLOCKSF;

	// Clear static lighting render-target
	context->setRenderTarget(m_staticLightingRenderTarget);
	context->disable(GraphicsContext::BLEND);
	context->setShader(m_initBlockLightingShader);
	context->drawRectangle(0.0f, 0.0f, width, height);
	
	// Draw static light sources to global render-target
	context->enable(GraphicsContext::BLEND);
	context->setShader(m_radialLightingShader);
	context->setBlendState(BlendState::PRESET_ADDITIVE);
	for(LightSource *light : m_lightSources)
	{
		if(light->getType() != LightSource::STATIC) continue;

		m_radialLightingShader->setUniform2f("u_Radius", light->getRadius() / width, light->getRadius() / height);
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
				context->drawCircleGradient(drawPos, light->getRadius(), light->getRadius() * 1.5f, light->getColor(), Color(0, 0, 0, 0));
			}
		}
	}
	context->disable(GraphicsContext::BLEND);

	// Blur horizontally (pass 0)
	m_blurHShader->setSampler2D("u_Texture", m_staticLightingRenderTarget->getTexture());
	context->setRenderTarget(m_lightingPass0);
	context->setShader(m_blurHShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Blur vertically (pass 1)
	m_blurVShader->setSampler2D("u_Texture", m_lightingPass0->getTexture());
	context->setRenderTarget(m_lightingPass1);
	context->setShader(m_blurVShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Reset graphics context
	context->enable(GraphicsContext::BLEND);
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