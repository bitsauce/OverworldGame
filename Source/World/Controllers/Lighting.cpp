#include "Lighting.h"
#include "Constants.h"
#include "World/World.h"

Lighting::Lighting(World *world) :
	m_world(world),
	m_enabled(true),
	m_redraw(false),
	m_lightingPass0(0),
	m_lightingPass1(0),
	m_lightingPass2(0),
	m_occludersRenderTarget(0),
	m_shadowMapRenderTarget(0),
	m_shadowsRenderTarget(0)
{
	ResourceManager *resourceManager = Game::GetInstance()->getResourceManager();
	m_shadowMapShader = resourceManager->get<Shader>("Shaders/Shadow_Map");
	m_shadowRenderShader = resourceManager->get<Shader>("Shaders/Shadow_Render");

	setLightMapResolution(512);
}

Lighting::~Lighting()
{
}

void Lighting::addLightSource(LightSource *source)
{
	m_lightSources.push_back(source);
	m_redraw = true;
}

void Lighting::removeLightSource(LightSource *lightSource)
{
	m_lightSources.remove(lightSource);
}

void Lighting::onDraw(DrawEvent *e)
{
	if(!m_enabled) return;

	ChunkManager *chunkManager = m_world->getTerrain()->getChunkManager();
	const float width = chunkManager->m_loadingArea.getWidth() * CHUNK_BLOCKSF, height = chunkManager->m_loadingArea.getHeight() * CHUNK_BLOCKSF;

	GraphicsContext *context = e->getGraphicsContext();
	Vector2F viewSize = m_world->getCamera()->getSize();

	// Draw block lighting to lighting pass 0
	context->setShader(0);
	context->disable(GraphicsContext::BLEND);
	context->setRenderTarget(m_lightingPass0);
	context->setTexture(chunkManager->m_blockLightingRenderTarget->getTexture());
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Blur horizontally (target: pass 1)
	context->setRenderTarget(m_lightingPass1);
	context->setShader(chunkManager->m_blurHShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Blur vertically (target: pass 2)
	context->setRenderTarget(m_lightingPass2);
	context->setShader(chunkManager->m_blurVShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Calculate view UVs
	ChunkManager::ChunkArea area = m_world->getTerrain()->getChunkManager()->getLoadingArea();
	Vector2F cameraPosition = m_world->getCamera()->getDrawPosition(e->getAlpha()), cameraSize = m_world->getCamera()->getSize();
	float u0 = cameraPosition.x / (area.getWidth() * CHUNK_PXF),
		v0 = cameraPosition.y / (area.getHeight() * CHUNK_PXF),
		u1 = (cameraPosition.x + cameraSize.x) / (area.getWidth() * CHUNK_PXF),
		v1 = (cameraPosition.y + cameraSize.y) / (area.getHeight() * CHUNK_PXF);

	// Draw static lighting result to the dynamic lighting render target
	context->setRenderTarget(m_shadowsRenderTarget);
	context->setShader(0);
	context->setTexture(m_lightingPass2->getTexture());
	context->drawRectangle(0, 0, m_shadowsRenderTarget->getWidth(), m_shadowsRenderTarget->getHeight(), Color(255), TextureRegion(u0, v0, u1, v1));
	context->setTexture(0);
	context->enable(GraphicsContext::BLEND);

	// Draw dynamic light sources
	for(LightSource *l : m_lightSources)
	{
		if(l->getType() != LightSource::DYNAMIC) continue;
		drawLight(l, m_shadowsRenderTarget, context);
	}

	// Draw shadows
	context->setRenderTarget(0);
	context->setBlendState(BlendState::PRESET_MULTIPLY);
	context->setTexture(m_shadowsRenderTarget->getTexture());
	context->drawRectangle(0, 0, viewSize.x, viewSize.y);
	context->setBlendState(BlendState::PRESET_ALPHA_BLEND);
}

void Lighting::drawLight(LightSource *light, RenderTarget2D *dest, GraphicsContext *context)
{
	context->disable(GraphicsContext::BLEND);

	ChunkManager *chunkManager = m_world->getTerrain()->getChunkManager();
	float radius = light->getRadius() * BLOCK_PXF;
	Vector2F position = light->getPosition() * BLOCK_PXF - m_world->getCamera()->getPosition();

	// Draw occluders to render target
	context->setRenderTarget(m_occludersRenderTarget);
	context->clear(GraphicsContext::COLOR_BUFFER);
	context->setTexture(chunkManager->m_shadowCasterRenderTarget->getTexture());
	context->drawRectangle(((Vector2F(radius * 0.5f) - position) / radius) * m_lightMapSize, chunkManager->m_shadowCasterRenderTarget->getTexture()->getSize() * m_lightMapSize / radius);

	// Create 1D shadow map
	context->setRenderTarget(m_shadowMapRenderTarget);
	context->setShader(m_shadowMapShader);
	m_shadowMapShader->setUniform2f("u_Resolution", m_lightMapSize, m_lightMapSize);
	m_shadowMapShader->setUniform1f("u_Scale", 1.0f);
	m_shadowMapShader->setSampler2D("u_Texture", m_occludersRenderTarget->getTexture());
	context->drawRectangle(0.0f, 0.0f, m_lightMapSize, m_shadowMapRenderTarget->getHeight());

	// Render the shadows
	context->setRenderTarget(dest);
	context->enable(GraphicsContext::BLEND);
	context->setBlendState(BlendState::PRESET_ADDITIVE);
	context->setShader(m_shadowRenderShader);
	//m_shadowRenderShader->setUniform2f("u_Resolution", m_lightMapSize, m_lightMapSize);
	Color color = light->getColor();
	m_shadowRenderShader->setUniform3f("u_Color", color.getR() / 255.0f, color.getG() / 255.0f, color.getB() / 255.0f);
	m_shadowRenderShader->setSampler2D("u_Texture", m_shadowMapRenderTarget->getTexture());
	m_shadowRenderShader->setSampler2D("u_OccluderTexture", m_occludersRenderTarget->getTexture());
	context->drawRectangle(position - Vector2F(radius * 0.5f), Vector2F(radius));

	// Reset
	context->setBlendState(BlendState::PRESET_ALPHA_BLEND);
	context->setShader(0);
	context->setRenderTarget(0);
	context->setTexture(0);
}

void Lighting::setLightMapResolution(const int size)
{
	Vector2I viewSize = m_world->getCamera()->getSize();

	delete m_occludersRenderTarget;
	delete m_shadowMapRenderTarget;
	delete m_shadowsRenderTarget;

	m_occludersRenderTarget = new RenderTarget2D(size, size);
	m_shadowMapRenderTarget = new RenderTarget2D(size, 1);
	m_shadowsRenderTarget = new RenderTarget2D(viewSize.x, viewSize.y);
	m_shadowMapRenderTarget->getTexture()->setWrapping(Texture2D::REPEAT);

	m_lightMapSize = size;
}