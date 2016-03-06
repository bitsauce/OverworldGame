#ifndef LIGHTING_H
#define LIGHTING_H

#include "Config.h"

#include "Lighting/LightSource.h"
#include "Lighting/Pointlight.h"

class Terrain;
class World;

class Lighting : public SceneObject
{
	friend class ChunkManager;
	friend class Debug;
public:
	// CONSTRUCTOR
	Lighting(World *world);
	~Lighting();

	// DRAWING
	void onDraw(DrawEvent *e);
	
	void addLightSource(LightSource *lightSource);

private:
	World *m_world;

	bool m_enabled;
	bool m_redrawLighting;

	list<LightSource*> m_lightSources;
};

#endif // LIGHTING_H