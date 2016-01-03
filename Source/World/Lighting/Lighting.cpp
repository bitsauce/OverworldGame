#include "Lighting.h"
#include "Constants.h"
#include "Pointlight.h"
#include "World/World.h"
#include "World/TimeOfDay.h"
#include "World/Camera.h"
#include "World/Terrain.h"
#include "World/ChunkManager.h"

Lighting::Lighting(World *world) :
	Entity(world, ENTITY_LIGHTING),
	m_terrain(world->getTerrain()),
	m_world(world),
	m_enabled(true)
{
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

	GraphicsContext *context = e->getGraphicsContext();
	
	/*ChunkManager::ChunkArea area = m_terrain->getChunkManager()->getLoadingArea();

	// Re-enable alpha blending
	context->setBlendState(BlendState::PRESET_MULTIPLY);

	context->setViewMatrix(m_world->getCamera()->getTransformationMatrix(e->getAlpha()));
	context->setTexture(m_terrain->getChunkManager()->m_lightingPass2->getTexture());

	float u0 = area.x0 / (float) area.getWidth(),
		v0 = -area.y0 / (float) area.getHeight(),
		u1 = u0 + 1.0f,
		v1 = v0 + 1.0f;

	context->drawRectangle(area.x0 * CHUNK_PXF, area.y0 * CHUNK_PXF, area.getWidth() * CHUNK_PXF, area.getHeight() * CHUNK_PXF, Color(255), TextureRegion(u0, v0, u1, v1));
	context->setBlendState(BlendState::PRESET_ALPHA_BLEND);*/
}