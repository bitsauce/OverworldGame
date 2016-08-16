#ifndef LIGHTING_H
#define LIGHTING_H

#include "Config.h"

#include "LightSource.h"
#include "Pointlight.h"

class Terrain;
class World;

class Lighting : public SceneObject
{
	friend class ChunkManager;
	friend class Debug;
	friend class LightSource;
public:
	// CONSTRUCTOR
	Lighting(World *world);
	~Lighting();

	// DRAWING
	void onDraw(DrawEvent *e);

private:
	World *m_world;

	bool m_enabled;
	bool m_redraw;

	RenderTarget2D *m_lightingPass0;
	RenderTarget2D *m_lightingPass1;
	RenderTarget2D *m_lightingPass2;
	RenderTarget2D* m_occludersRenderTarget;
	RenderTarget2D* m_shadowMapRenderTarget;
	RenderTarget2D* m_shadowsRenderTarget;

	Resource<Shader> m_shadowMapShader;
	Resource<Shader> m_shadowRenderShader;

	int m_lightMapSize;

	list<LightSource*> m_lightSources;

	void addLightSource(LightSource *lightSource);
	void removeLightSource(LightSource *lightSource);

	void setLightMapResolution(const int size);
	void drawLight(LightSource *light, RenderTarget2D *dest, GraphicsContext *context);
};

#endif // LIGHTING_H