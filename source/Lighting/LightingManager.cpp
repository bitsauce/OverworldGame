#include "Game.h"

LightingManager::LightingManager(Terrain *terrain) :
	GameObject(DRAW_ORDER_LIGHTING),
	m_terrain(terrain),
	m_lightingPass0(nullptr),
	m_lightingPass1(nullptr),
	m_lightingPass2(nullptr),
	m_lightingRenderTarget(nullptr),
	m_directionalLightingShader(xd::ResourceManager::get<xd::Shader>(":/shaders/directional_lighting")),
	m_blurHShader(xd::ResourceManager::get<xd::Shader>(":/shaders/blur_h")),
	m_blurVShader(xd::ResourceManager::get<xd::Shader>(":/shaders/blur_v")),
	m_lightRadius(6)
{
	// Resize render targets
	XWindow::addWindowListener(this);
	resizeEvent(XWindow::getSize().x, XWindow::getSize().y);
}

LightingManager::~LightingManager()
{
	XWindow::removeWindowListener(this);
}

void LightingManager::addLightSource(LightSource *source)
{
	m_lightSources.push_back(source);
}

void LightingManager::draw(xd::SpriteBatch *spriteBatch)
{
	xd::GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();

	// Get chunk coordinates
	int x0 = (int)floor(World::getCamera()->getX()/CHUNK_PXF);
	int y0 = (int)floor(World::getCamera()->getY()/CHUNK_PXF);
	int x1 = (int)floor((World::getCamera()->getX() + World::getCamera()->getWidth())/CHUNK_PXF);
	int y1 = (int)floor((World::getCamera()->getY() + World::getCamera()->getHeight())/CHUNK_PXF);

	// Draw shadows
	//if(m_prevX0 != x0 || m_prevY0 != y0)
	{
		// Reset projection matrix
		gfxContext.setProjectionMatrix(Matrix4());

		// Disable alpha blend when drawing to render targets
		gfxContext.disable(xd::GraphicsContext::BLEND);

		// Render lightmaps to texture (pass 0)
		gfxContext.setRenderTarget(m_lightingRenderTarget);
		gfxContext.clear(xd::GraphicsContext::COLOR_BUFFER);
		for(int y = y0-2; y <= y1+2; ++y)
		{
			for(int x = x0-2; x <= x1+2; ++x)
			{
				gfxContext.setTexture(m_terrain->getChunk(x, y, true).getLightMap());
				gfxContext.drawRectangle((x - x0 + 1) * CHUNK_BLOCKSF, (y - y0 + 1) * CHUNK_BLOCKSF, CHUNK_BLOCKSF, CHUNK_BLOCKSF);
			}
		}
		
		for(list<LightSource*>::iterator itr = m_lightSources.begin(); itr != m_lightSources.end(); ++itr)
		{
			gfxContext.drawCircle((*itr)->getPosition().x, (*itr)->getPosition().y, (*itr)->getRadius(), 32, xd::Color(0));
		}

		gfxContext.drawCircle(CHUNK_BLOCKS*2 + XInput::getPosition().x/BLOCK_PXF, CHUNK_BLOCKS*2 + XInput::getPosition().y/BLOCK_PXF, 2, 32, xd::Color(0));

		// Directional light
		gfxContext.setRenderTarget(m_lightingPass0);
		gfxContext.clear(xd::GraphicsContext::COLOR_BUFFER);
		m_directionalLightingShader->setSampler2D("u_texture", m_lightingRenderTarget->getTexture());
		m_directionalLightingShader->setUniform1f("u_direction", 0.0174532925f * 180.0f * (World::getTimeOfDay()->isDay() ? (1140.0f - World::getTimeOfDay()->getTime()) : (1860.0f - (World::getTimeOfDay()->getTime() >= 1140.0f ? World::getTimeOfDay()->getTime() : World::getTimeOfDay()->getTime() + 1440.0f)))/720.0f);
		m_directionalLightingShader->setUniform1f("u_offsetY", floor(World::getCamera()->getY()/CHUNK_BLOCKSF)/m_height + (y0*CHUNK_BLOCKSF - floor(World::getCamera()->getY()/CHUNK_BLOCKSF))/m_height - 32.0f/m_height);
		m_directionalLightingShader->setUniform1f("u_width", m_width);
		m_directionalLightingShader->setUniform1f("u_height", m_height);
		gfxContext.setShader(m_directionalLightingShader);
		gfxContext.drawRectangle(0.0f, 0.0f, (float)m_width, (float)m_height);

		// Blur horizontally (pass 1)
		gfxContext.setRenderTarget(m_lightingPass1);
		gfxContext.clear(xd::GraphicsContext::COLOR_BUFFER);
		m_blurHShader->setSampler2D("u_texture", m_lightingPass0->getTexture());
		m_blurHShader->setUniform1i("u_width", m_width);
		gfxContext.setShader(m_blurHShader);
		gfxContext.drawRectangle(0.0f, 0.0f, (float)m_width, (float)m_height);

		// Blur vertically (pass 2)
		gfxContext.setRenderTarget(m_lightingPass2);
		gfxContext.clear(xd::GraphicsContext::COLOR_BUFFER);
		m_blurVShader->setSampler2D("u_texture", m_lightingPass1->getTexture());
		m_blurVShader->setUniform1i("u_height", m_height);
		m_blurVShader->setUniform1f("u_exponent", 2.0);
		gfxContext.setShader(m_blurVShader);
		gfxContext.drawRectangle(0.0f, 0.0f, (float)m_width, (float)m_height);
		
		// Re-enable alpha blending
		gfxContext.setRenderTarget(nullptr);
		gfxContext.setShader(nullptr);
		
		gfxContext.enable(xd::GraphicsContext::BLEND);
		gfxContext.setBlendState(xd::BlendState::PRESET_MULTIPLY);

		gfxContext.setProjectionMatrix(World::getCamera()->getProjectionMatrix());
		gfxContext.setTexture(m_lightingPass2->getTexture());
		gfxContext.drawRectangle((x0 - 1) * CHUNK_PXF, (y0 - 1) * CHUNK_PXF, m_width * BLOCK_PXF, m_height * BLOCK_PXF);
		
		gfxContext.setBlendState(xd::BlendState::PRESET_ALPHA_BLEND);
	}
}

void LightingManager::resizeEvent(uint width, uint height)
{
	// Clear old render targets
	delete m_lightingPass0;
	delete m_lightingPass1;
	delete m_lightingPass2;
	delete m_lightingRenderTarget;

	// Create shadow textures
	uint targetWidth = (uint)(floor(width/CHUNK_PXF) + 4) * CHUNK_BLOCKS;
	uint targetHeight = (uint)(floor(height/CHUNK_PXF) + 4) * CHUNK_BLOCKS;
	m_lightingRenderTarget = new xd::RenderTarget2D(targetWidth, targetHeight);
	m_lightingPass0 = new xd::RenderTarget2D(targetWidth, targetHeight);
	m_lightingPass1 = new xd::RenderTarget2D(targetWidth, targetHeight);
	m_lightingPass2 = new xd::RenderTarget2D(targetWidth, targetHeight);
	m_lightingPass2->getTexture()->setFiltering(xd::Texture2D::LINEAR);

	// Set size
	m_width = targetWidth;
	m_height = targetHeight;

	// Make sure the shadows will update
	//m_prevX0 = m_prevY0 = 0xFFFFFFFF;
}