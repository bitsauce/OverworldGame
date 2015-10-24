#ifndef LIGHTING_H
#define LIGHTING_H

#include "Config.h"
#include "LightSource.h"
#include "Pointlight.h"

class Terrain;
class World;

class Lighting : public WindowListener
{
	friend class ChunkLoader;
	friend class Debug;
public:
	// CONSTRUCTOR
	Lighting(World *world);
	~Lighting();

	// DRAWING
	void draw(xd::GraphicsContext &gfxContext);

	// WINDOW
	void resizeEvent(uint width, uint height);
	
	void addLightSource(LightSource *lightSource);

	void draw(xd::SpriteBatch *spriteBatch, const float alpha);

private:
	Terrain *m_terrain;
	World *m_world;

	bool m_enabled;
	bool m_redrawLighting;

	list<LightSource*> m_lightSources;
};

#endif // LIGHTING_H