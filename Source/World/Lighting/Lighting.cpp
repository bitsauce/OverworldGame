#include "Lighting.h"
#include "Constants.h"
#include "Pointlight.h"
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
	m_directionalLightingShader(ResourceManager::get<Shader>(":/Shaders/DirectionalLighting")),
	m_radialLightingShader(ResourceManager::get<Shader>(":/Shaders/RadialLighting")),
	m_blurHShader(ResourceManager::get<Shader>(":/Shaders/BlurH")),
	m_blurVShader(ResourceManager::get<Shader>(":/Shaders/BlurV")),
	m_lightRadius(6),
	m_enabled(true),
	m_redrawLighting(true)
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

void Lighting::draw(SpriteBatch *spriteBatch, const float alpha)
{
	if(!m_enabled) return;

	GraphicsContext &context = spriteBatch->getGraphicsContext();

	ChunkLoader::ChunkArea area = m_terrain->getChunkLoader()->getLoadingArea();

	// Draw shadows
	//if(m_redrawLighting)
	{
		// Reset model-view matrix
		context.setModelViewMatrix(Matrix4());

		// Disable alpha blend when drawing to render targets
		context.disable(GraphicsContext::BLEND);

		// Directional light
		context.setRenderTarget(m_lightingPass0);
		m_directionalLightingShader->setSampler2D("u_Texture", m_world->getTerrain()->getChunkLoader()->m_blocksRenderTarget->getTexture());
		m_directionalLightingShader->setUniform1f("u_Direction", 0.0174532925f * 180.0f * (m_world->getTimeOfDay()->isDay() ? (1140.0f - m_world->getTimeOfDay()->getTime()) : (1860.0f - (m_world->getTimeOfDay()->getTime() >= 1140.0f ? m_world->getTimeOfDay()->getTime() : m_world->getTimeOfDay()->getTime() + 1440.0f))) / 720.0f);
		m_directionalLightingShader->setUniform1f("u_OffsetY", (area.y0 * CHUNK_BLOCKSF - 32.0f) / m_height);
		m_directionalLightingShader->setUniform1f("u_Width", m_width);
		m_directionalLightingShader->setUniform1f("u_Height", m_height);
		context.setShader(m_directionalLightingShader);
		context.drawRectangle(0.0f, 0.0f, (float) m_width, (float) m_height);

		// Draw light sources
		context.enable(GraphicsContext::BLEND);
		context.setShader(m_radialLightingShader);
		context.setBlendState(BlendState::PRESET_ADDITIVE);
		for(LightSource *light : m_lightSources)
		{
			Vector2 pos = light->getPosition() - Vector2(area.x0, area.y0) * CHUNK_BLOCKSF + Vector2(0.5f, 0.5f);
			m_radialLightingShader->setSampler2D("u_LightMap", m_world->getTerrain()->getChunkLoader()->m_blocksRenderTarget->getTexture());
			m_radialLightingShader->setUniform2f("u_LightTexCoord", pos.x / m_width, 1.0f - (pos.y / m_height));
			m_radialLightingShader->setUniform2f("u_Radius", light->getRadius() / m_width, light->getRadius() / m_height);
			m_radialLightingShader->setUniform1i("u_Iterations", 20);
			m_radialLightingShader->setUniform3f("u_Color", light->getColor().r / 255.0f, light->getColor().g / 255.0f, light->getColor().b / 255.0f);
			context.drawCircle(pos, light->getRadius(), light->getRadius() * 1.5f);
		}
		context.disable(GraphicsContext::BLEND);

		// Blur horizontally (pass 1)
		context.setRenderTarget(m_lightingPass1);
		m_blurHShader->setSampler2D("u_Texture", m_lightingPass0->getTexture());
		m_blurHShader->setUniform1i("u_Width", m_width);
		context.setShader(m_blurHShader);
		context.drawRectangle(0.0f, 0.0f, (float) m_width, (float) m_height);

		// Blur vertically (pass 2)
		context.setRenderTarget(m_lightingPass2);
		m_blurVShader->setSampler2D("u_Texture", m_lightingPass1->getTexture());
		m_blurVShader->setUniform1i("u_Height", m_height);
		context.setShader(m_blurVShader);
		context.drawRectangle(0.0f, 0.0f, (float) m_width, (float) m_height);

		// Re-enable alpha blending
		context.setRenderTarget(nullptr);
		context.setShader(nullptr);

		context.enable(GraphicsContext::BLEND);
		context.setBlendState(BlendState::PRESET_MULTIPLY);

		context.setModelViewMatrix(m_world->getCamera()->getModelViewMatrix(alpha));
		context.setTexture(m_lightingPass2->getTexture());

		Vector2 position((area.x0) * CHUNK_PXF, (area.y0) * CHUNK_PXF);
		Vector2 size(m_width * BLOCK_PXF, m_height * BLOCK_PXF);
		TextureRegion textureRegion(0,1,1,0);

		//VertexFormat format;
		//format.set(VertexAttribute::VERTEX_POSITION, 2);
		//format.set(VertexAttribute::VERTEX_TEX_COORD, 2);

		//Vertex *vertices = format.createVertices(4);
		Vertex vertices[4];
		vertices[0].set4f(VERTEX_POSITION, position.x, position.y);
		vertices[1].set4f(VERTEX_POSITION, position.x + size.x, position.y);
		vertices[2].set4f(VERTEX_POSITION, position.x, position.y + size.y);
		vertices[3].set4f(VERTEX_POSITION, position.x + size.x, position.y + size.y);

		Color color(255);

		vertices[0].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
		vertices[1].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
		vertices[2].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
		vertices[3].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);

		vertices[0].set4f(VERTEX_TEX_COORD, textureRegion.uv0.x, textureRegion.uv1.y);
		vertices[1].set4f(VERTEX_TEX_COORD, textureRegion.uv1.x, textureRegion.uv1.y);
		vertices[2].set4f(VERTEX_TEX_COORD, textureRegion.uv0.x, textureRegion.uv0.y);
		vertices[3].set4f(VERTEX_TEX_COORD, textureRegion.uv1.x, textureRegion.uv0.y);
		context.drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, vertices, 4);
		//delete[] vertices;

		context.setBlendState(BlendState::PRESET_ALPHA_BLEND);
	}
}

void Lighting::resizeEvent(uint width, uint height)
{
	// Clear old render targets
	delete m_lightingPass0;
	delete m_lightingPass1;
	delete m_lightingPass2;

	// Create shadow textures
	int targetWidth = ((int) (floor(width * 0.5f / CHUNK_PXF) * 2 + 3) + 5 * 2) * CHUNK_BLOCKS;
	int targetHeight = ((int) (floor(height * 0.5f / CHUNK_PXF) * 2 + 3) + 5 * 2) * CHUNK_BLOCKS;
	m_lightingPass0 = new RenderTarget2D(targetWidth, targetHeight);
	m_lightingPass0->getTexture()->setFiltering(Texture2D::LINEAR);
	m_lightingPass1 = new RenderTarget2D(targetWidth, targetHeight);
	m_lightingPass1->getTexture()->setFiltering(Texture2D::LINEAR);
	m_lightingPass2 = new RenderTarget2D(targetWidth, targetHeight);
	m_lightingPass2->getTexture()->setFiltering(Texture2D::LINEAR);

	// Set size
	m_width = targetWidth;
	m_height = targetHeight;
}