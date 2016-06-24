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
	bool m_redraw;

	void setLightMapResolution(const int size);
	void drawLight(LightSource *light, RenderTarget2D *dest, GraphicsContext *context);

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
};

#endif // LIGHTING_H