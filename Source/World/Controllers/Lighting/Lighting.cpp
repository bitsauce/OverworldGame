#include "Lighting.h"
#include "Constants.h"
#include "World/World.h"

Lighting::Lighting(World *world, Window *window) :
	m_world(world),
	m_enabled(true),
	m_redrawStaticLighting(false),
	m_staticLightingRenderTarget(0),
	m_dynamicLightingRenderTarget(0),
	m_dynamicLightingBlurH(0),
	m_dynamicLightingBlurV(0),
	m_initBlockLightingShader(Resource<Shader>("Shaders/Init_Block_Lighting")),
	m_radialLightingShader(Resource<Shader>("Shaders/RadialLighting")),
	m_blurHShader(Resource<Shader>("Shaders/BlurH")),
	m_blurVShader(Resource<Shader>("Shaders/BlurV"))
{
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

	ChunkManager::ChunkArea loadingArea = m_world->getTerrain()->getChunkManager()->getLoadingArea();
	const float width = loadingArea.getWidth() * CHUNK_BLOCKSF, height = loadingArea.getHeight() * CHUNK_BLOCKSF;

	// Redraw static lighting if need be
	if(m_redrawStaticLighting)
	{
		// Draw block light map to static lighting
		context->setRenderTarget(m_staticLightingRenderTarget);
		context->disable(GraphicsContext::BLEND);
		context->setShader(m_initBlockLightingShader);
		context->drawRectangle(0.0f, 0.0f, width, height);

		// Draw static light sources
		drawLightSources(LightSource::STATIC, context);

		// Reset redraw flag
		m_redrawStaticLighting = false;
	}

	context->disable(GraphicsContext::BLEND);

	// Clear dynamic lighting
	context->setRenderTarget(m_dynamicLightingRenderTarget);
	context->drawRectangle(0.0f, 0.0f, width, height, Color::Black);

	// Draw dynamic light sources
	drawLightSources(LightSource::DYNAMIC, context);

	// Add static lighting to render-target before blurring
	context->setBlendState(BlendState::PRESET_ADDITIVE);
	context->setShader(0);
	context->setTexture(m_staticLightingRenderTarget->getTexture());
	context->drawRectangle(0.0f, 0.0f, width, height);

	context->disable(GraphicsContext::BLEND);

	// Blur horizontally (pass 0)
	context->setRenderTarget(m_dynamicLightingBlurH);
	context->setShader(m_blurHShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Blur vertically (pass 1)
	context->setRenderTarget(m_dynamicLightingBlurV);
	context->setShader(m_blurVShader);
	context->drawRectangle(0.0f, 0.0f, width, height);

	// Draw final light render target over the screen
	context->enable(GraphicsContext::BLEND);
	context->setRenderTarget(0);
	context->setBlendState(BlendState::PRESET_MULTIPLY);
	context->setTransformationMatrix(m_world->getCamera()->getTransformationMatrix(e->getAlpha()));
	context->setTexture(m_dynamicLightingBlurV->getTexture());

	float u0 = loadingArea.x0 / (float) loadingArea.getWidth(),
		v0 = loadingArea.y0 / (float) loadingArea.getHeight(),
		u1 = u0 + 1.0f,
		v1 = v0 + 1.0f;

	context->drawRectangle(loadingArea.x0 * CHUNK_PXF, loadingArea.y0 * CHUNK_PXF, loadingArea.getWidth() * CHUNK_PXF, loadingArea.getHeight() * CHUNK_PXF, Color::White, TextureRegion(u0, v0, u1, v1));
	context->setBlendState(BlendState::PRESET_ALPHA_BLEND);
	context->setShader(0);
	context->setTexture(0);
}

void Lighting::drawLightSources(LightSource::Mobility mobility, GraphicsContext *context)
{
	ChunkManager::ChunkArea loadingArea = m_world->getTerrain()->getChunkManager()->getLoadingArea();
	const float width = loadingArea.getWidth() * CHUNK_BLOCKSF, height = loadingArea.getHeight() * CHUNK_BLOCKSF;
	
	// Draw static light sources to static lighting render-target
	context->enable(GraphicsContext::BLEND);
	context->setBlendState(BlendState::PRESET_ADDITIVE);
	context->setShader(0);
	for(LightSource *light : m_lightSources)
	{
		if(light->getMobility() == mobility)
		{
			if(light->getType() == LightSource::POINTLIGHT)
			{
				context->setShader(m_radialLightingShader);
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

						// Apply transformation and draw circle gradient
						Matrix4 mat;
						mat.scale(light->getRadius());
						mat.translate(drawPos.x, drawPos.y, 0.0);
						context->pushMatrix(mat);
						context->drawCircleGradient(0.0f, 0.0f, 1.0f, light->getRadius() * 1.5f, light->getColor(), Color::Black);
						context->popMatrix();
					}
				}
			}
			else
			{
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

						Spotlight *spotlight = (Spotlight*) light;

						Vertex *vertices = context->getVertices(5);

						vertices[0].set2f(VERTEX_POSITION, 0.0f, 0.0f);
						vertices[0].set4ub(VERTEX_COLOR, 255, 255, 255, 255);

						float f2 = spotlight->getDirection() - spotlight->getConeAngle();
						float f3 = spotlight->getDirection() + spotlight->getConeAngle();
						vertices[2].set2f(VERTEX_POSITION, cos(f2), sin(f2));
						vertices[2].set4ub(VERTEX_COLOR, 0, 0, 0, 0);
						vertices[3].set2f(VERTEX_POSITION, cos(f3), sin(f3));
						vertices[3].set4ub(VERTEX_COLOR, 0, 0, 0, 0);

						vertices[1].set2f(VERTEX_POSITION, cos(spotlight->getDirection() + PI) * 0.5f, sin(spotlight->getDirection() + PI) * 0.5f);
						vertices[1].set4ub(VERTEX_COLOR, 0, 0, 0, 0);
						vertices[4].set2f(VERTEX_POSITION, cos(spotlight->getDirection() + PI) * 0.5f, sin(spotlight->getDirection() + PI) * 0.5f);
						vertices[4].set4ub(VERTEX_COLOR, 0, 0, 0, 0);

						Matrix4 mat;
						mat.scale(light->getRadius());
						mat.translate(drawPos.x, drawPos.y, 0.0);
						context->pushMatrix(mat);
						context->drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_FAN, &vertices[0], 5);
						context->popMatrix();
					}
				}
			}
		}
	}
	
	// Reset graphics context
	context->setShader(0);
	context->setBlendState(BlendState::PRESET_ALPHA_BLEND);
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

	delete m_staticLightingRenderTarget;
	delete m_dynamicLightingRenderTarget;
	delete m_dynamicLightingBlurH;
	delete m_dynamicLightingBlurV;

	m_staticLightingRenderTarget = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_staticLightingRenderTarget->getTexture()->setFiltering(Texture2D::LINEAR);
	m_staticLightingRenderTarget->getTexture()->setWrapping(Texture2D::REPEAT);
	m_dynamicLightingRenderTarget = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_dynamicLightingRenderTarget->getTexture()->setFiltering(Texture2D::LINEAR);
	m_dynamicLightingRenderTarget->getTexture()->setWrapping(Texture2D::REPEAT);
	m_dynamicLightingBlurH = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_dynamicLightingBlurH->getTexture()->setFiltering(Texture2D::LINEAR);
	m_dynamicLightingBlurH->getTexture()->setWrapping(Texture2D::REPEAT);
	m_dynamicLightingBlurV = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_dynamicLightingBlurV->getTexture()->setFiltering(Texture2D::LINEAR);
	m_dynamicLightingBlurV->getTexture()->setWrapping(Texture2D::REPEAT);


	// Create shadow textures
	m_staticLightingRenderTarget = new RenderTarget2D(loadAreaWidth * CHUNK_BLOCKS, loadAreaHeight * CHUNK_BLOCKS);
	m_staticLightingRenderTarget->getTexture()->setFiltering(Texture2D::LINEAR);
	m_staticLightingRenderTarget->getTexture()->setWrapping(Texture2D::REPEAT);

	// Set uniforms
	m_initBlockLightingShader->setSampler2D("u_Texture", chunkManager->m_blocksRenderTarget->getTexture());
	m_radialLightingShader->setSampler2D("u_LightMap", chunkManager->m_blocksRenderTarget->getTexture());
	m_radialLightingShader->setUniform1i("u_Iterations", 32);
	m_blurHShader->setUniform1i("u_Width", loadAreaWidth * CHUNK_BLOCKS);
	m_blurHShader->setSampler2D("u_Texture", m_dynamicLightingRenderTarget->getTexture());
	m_blurVShader->setUniform1i("u_Height", loadAreaHeight * CHUNK_BLOCKS);
	m_blurVShader->setSampler2D("u_Texture", m_dynamicLightingBlurH->getTexture());
}