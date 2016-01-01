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

void Lighting::onDraw(DrawEvent *e)
{
	if(!m_enabled) return;

	GraphicsContext &context = spriteBatch->getGraphicsContext();

	ChunkLoader::ChunkArea area = m_terrain->getChunkLoader()->getLoadingArea();

	// Re-enable alpha blending
	context.setBlendState(BlendState::PRESET_MULTIPLY);

	context.setModelViewMatrix(m_world->getCamera()->getModelViewMatrix(alpha));
	context.setTexture(m_terrain->getChunkLoader()->m_lightingPass2->getTexture());

	float u0 = area.x0 / (float) area.getWidth(),
		v0 = -area.y0 / (float) area.getHeight(),
		u1 = u0 + 1.0f,
		v1 = v0 + 1.0f;

	context.drawRectangle(area.x0 * CHUNK_PXF, area.y0 * CHUNK_PXF, area.getWidth() * CHUNK_PXF, area.getHeight() * CHUNK_PXF, Color(255), TextureRegion(u0, v0, u1, v1));
	context.setBlendState(BlendState::PRESET_ALPHA_BLEND);
}

void Lighting::resizeEvent(uint width, uint height)
{
}