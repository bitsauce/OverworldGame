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

void Lighting::onDraw(DrawEvent *e)
{
	if(!m_enabled) return;

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


void Lighting::addLightSource(LightSource *source)
{
	m_lightSources.push_back(source);
	m_redraw = true;
}

void Lighting::removeLightSource(LightSource *lightSource)
{
	m_lightSources.remove(lightSource);
}