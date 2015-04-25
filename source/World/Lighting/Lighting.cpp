#include "Lighting.h"
#include "Constants.h"
#include "Spotlight.h"
#include "World/World.h"
#include "World/TimeOfDay.h"
#include "World/Camera.h"
#include "World/Terrain/Terrain.h"
#include "World/Terrain/ChunkLoader.h"

Lighting::Lighting(World *world) :
	m_terrain(world->getTerrain()),
	m_world(world),
	m_lightingPass0(nullptr),
	m_lightingPass1(nullptr),
	m_lightingPass2(nullptr),
	m_lightingRenderTarget(nullptr),
	m_directionalLightingShader(ResourceManager::get<Shader>(":/shaders/directional_lighting")),
	m_radialLightingShader(ResourceManager::get<Shader>(":/shaders/radial_lighting")),
	m_blurHShader(ResourceManager::get<Shader>(":/shaders/blur_h")),
	m_blurVShader(ResourceManager::get<Shader>(":/shaders/blur_v")),
	m_lightRadius(6),
	m_enabled(true)
{
	// Resize render targets
	resizeEvent(Window::getSize().x, Window::getSize().y);
}

Lighting::~Lighting()
{
}

void Lighting::addLightSource(LightSource *source)
{
	m_lightSources.push_back(source);
}

void Lighting::draw(SpriteBatch *spriteBatch)
{
	if(!m_enabled) return;

	GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();

	ChunkLoader::ChunkArea area = m_terrain->getChunkLoader()->getActiveArea();

	// Draw shadows
	//if(m_prevX0 != x0 || m_prevY0 != y0)
	{
		// Reset projection matrix
		gfxContext.setProjectionMatrix(Matrix4());

		// Disable alpha blend when drawing to render targets
		gfxContext.disable(GraphicsContext::BLEND);

		// Render lightmaps to texture (pass 0)
		gfxContext.setRenderTarget(m_lightingRenderTarget);
		gfxContext.clear(GraphicsContext::COLOR_BUFFER);
		for(int y = area.y0; y <= area.y1+2; ++y)
		{
			for(int x = area.x0; x <= area.x1+2; ++x)
			{
				gfxContext.setTexture(m_terrain->getChunkLoader()->getChunkAt(x-1, y-1).getLightMap());
				gfxContext.drawRectangle((x - area.x0) * CHUNK_BLOCKSF, (y - area.y0) * CHUNK_BLOCKSF, CHUNK_BLOCKSF, CHUNK_BLOCKSF);
			}
		}
		
		// Directional light
		gfxContext.setRenderTarget(m_lightingPass0);
		gfxContext.clear(GraphicsContext::COLOR_BUFFER);
		m_directionalLightingShader->setSampler2D("u_texture", m_lightingRenderTarget->getTexture());
		m_directionalLightingShader->setUniform1f("u_direction", 0.0174532925f * 180.0f * (m_world->getTimeOfDay()->isDay() ? (1140.0f - m_world->getTimeOfDay()->getTime()) : (1860.0f - (m_world->getTimeOfDay()->getTime() >= 1140.0f ? m_world->getTimeOfDay()->getTime() : m_world->getTimeOfDay()->getTime() + 1440.0f)))/720.0f);
		m_directionalLightingShader->setUniform1f("u_offsetY", (area.y0 * CHUNK_BLOCKSF - 32.0f)/m_height);
		m_directionalLightingShader->setUniform1f("u_width", m_width);
		m_directionalLightingShader->setUniform1f("u_height", m_height);
		gfxContext.setShader(m_directionalLightingShader);
		gfxContext.drawRectangle(0.0f, 0.0f, (float)m_width, (float)m_height);
		
		// Draw light sources
		gfxContext.enable(GraphicsContext::BLEND);
		gfxContext.setShader(m_radialLightingShader);
		gfxContext.setBlendState(BlendState::PRESET_ADDITIVE);
		for(LightSource *light : m_lightSources)
		{
			Vector2 pos = light->getPosition() - Vector2(area.x0 - 1, area.y0 - 1) * CHUNK_BLOCKSF + Vector2(0.5f, 0.5f);
			m_radialLightingShader->setSampler2D("u_lightMap", m_lightingRenderTarget->getTexture());
			m_radialLightingShader->setUniform2f("u_lightTexCoord", pos.x/m_width, 1.0f - (pos.y/m_height));
			m_radialLightingShader->setUniform2f("u_radius", light->getRadius()/m_width, light->getRadius()/m_height);
			m_radialLightingShader->setUniform1i("u_iterations", 100);
			m_radialLightingShader->setUniform3f("u_color", light->getColor().r/255.0f, light->getColor().g/255.0f, light->getColor().b/255.0f);
			gfxContext.drawCircle(pos, light->getRadius(), light->getRadius()*1.5f);
		}
		gfxContext.disable(GraphicsContext::BLEND);

		// Blur horizontally (pass 1)
		gfxContext.setRenderTarget(m_lightingPass1);
		gfxContext.clear(GraphicsContext::COLOR_BUFFER);
		m_blurHShader->setSampler2D("u_texture", m_lightingPass0->getTexture());
		m_blurHShader->setUniform1i("u_width", m_width);
		gfxContext.setShader(m_blurHShader);
		gfxContext.drawRectangle(0.0f, 0.0f, (float)m_width, (float)m_height);

		// Blur vertically (pass 2)
		gfxContext.setRenderTarget(m_lightingPass2);
		gfxContext.clear(GraphicsContext::COLOR_BUFFER);
		m_blurVShader->setSampler2D("u_texture", m_lightingPass1->getTexture());
		m_blurVShader->setUniform1i("u_height", m_height);
		m_blurVShader->setUniform1f("u_exponent", 2.0);
		gfxContext.setShader(m_blurVShader);
		gfxContext.drawRectangle(0.0f, 0.0f, (float)m_width, (float)m_height);
		
		// Re-enable alpha blending
		gfxContext.setRenderTarget(nullptr);
		gfxContext.setShader(nullptr);
		
		gfxContext.enable(GraphicsContext::BLEND);
		gfxContext.setBlendState(BlendState::PRESET_MULTIPLY);

		gfxContext.setProjectionMatrix(m_world->getCamera()->getProjectionMatrix());
		gfxContext.setTexture(m_lightingPass2->getTexture());

		Vector2 position((area.x0) * CHUNK_PXF, (area.y0) * CHUNK_PXF);
		Vector2 size((m_width - 2 * CHUNK_BLOCKS) * BLOCK_PXF, (m_height - 2 * CHUNK_BLOCKS) * BLOCK_PXF);
		TextureRegion textureRegion(CHUNK_BLOCKSF/m_width, CHUNK_BLOCKSF/m_height, 1.0f - CHUNK_BLOCKSF/m_width, 1.0f - CHUNK_BLOCKSF/m_height);
		
		VertexFormat format;
		format.set(VertexAttribute::VERTEX_POSITION, 2);
		format.set(VertexAttribute::VERTEX_TEX_COORD, 2);

		Vertex *vertices = format.createVertices(4);
		vertices[0].set4f(VERTEX_POSITION, position.x,				position.y);
		vertices[1].set4f(VERTEX_POSITION, position.x + size.x,		position.y);
		vertices[2].set4f(VERTEX_POSITION, position.x,				position.y + size.y);
		vertices[3].set4f(VERTEX_POSITION, position.x + size.x,		position.y + size.y);
		vertices[0].set4f(VERTEX_TEX_COORD, textureRegion.uv0.x, textureRegion.uv1.y);
		vertices[1].set4f(VERTEX_TEX_COORD, textureRegion.uv1.x, textureRegion.uv1.y);
		vertices[2].set4f(VERTEX_TEX_COORD, textureRegion.uv0.x, textureRegion.uv0.y);
		vertices[3].set4f(VERTEX_TEX_COORD, textureRegion.uv1.x, textureRegion.uv0.y);
		gfxContext.drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, vertices, 4);
		delete[] vertices;
		
		gfxContext.setBlendState(BlendState::PRESET_ALPHA_BLEND);
	}
}

void Lighting::resizeEvent(uint width, uint height)
{
	// Clear old render targets
	delete m_lightingPass0;
	delete m_lightingPass1;
	delete m_lightingPass2;
	delete m_lightingRenderTarget;

	// Create shadow textures
	uint targetWidth = (uint)(floor(width*0.5f/CHUNK_PXF) * 2 + 5) * CHUNK_BLOCKS;
	uint targetHeight = (uint)(floor(height*0.5f/CHUNK_PXF) * 2 + 5) * CHUNK_BLOCKS;
	m_lightingRenderTarget = new RenderTarget2D(targetWidth, targetHeight);
	m_lightingRenderTarget->getTexture()->setFiltering(Texture2D::LINEAR);
	m_lightingPass0 = new RenderTarget2D(targetWidth, targetHeight);
	m_lightingPass0->getTexture()->setFiltering(Texture2D::LINEAR);
	m_lightingPass1 = new RenderTarget2D(targetWidth, targetHeight);
	m_lightingPass1->getTexture()->setFiltering(Texture2D::LINEAR);
	m_lightingPass2 = new RenderTarget2D(targetWidth, targetHeight);
	m_lightingPass2->getTexture()->setFiltering(Texture2D::LINEAR);

	// Set size
	m_width = targetWidth;
	m_height = targetHeight;

	// Make sure the shadows will update
	//m_prevX0 = m_prevY0 = 0xFFFFFFFF;
}