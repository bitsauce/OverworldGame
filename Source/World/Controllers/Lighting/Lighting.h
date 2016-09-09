#ifndef LIGHTING_H
#define LIGHTING_H

#include "Config.h"

#include "LightSource.h"
#include "Pointlight.h"
#include "Spotlight.h"

class Terrain;
class World;

class Lighting : public SceneObject
{
	friend class ChunkManager;
	friend class Debug;
	friend class LightSource;
public:
	// CONSTRUCTOR
	Lighting(World *world, Window *window);
	~Lighting();

	// DRAWING
	void onDraw(DrawEvent *e);
	void onWindowSizeChanged(WindowEvent *e);
private:
	World *m_world;

	bool m_enabled;
	bool m_redrawStaticLighting;

	RenderTarget2D *m_staticLightingRenderTarget;
	RenderTarget2D *m_dynamicLightingRenderTarget;
	RenderTarget2D *m_dynamicLightingBlurH;
	RenderTarget2D *m_dynamicLightingBlurV;

	Resource<Shader> m_initBlockLightingShader;
	Resource<Shader> m_radialLightingShader;
	Resource<Shader> m_blurHShader;
	Resource<Shader> m_blurVShader;

	list<LightSource*> m_lightSources;

	void addLightSource(LightSource *lightSource);
	void removeLightSource(LightSource *lightSource);

	void updateViewSize(int width, int height);

	void drawLightSources(LightSource::Mobility mobility, GraphicsContext *context);
};

#endif // LIGHTING_H