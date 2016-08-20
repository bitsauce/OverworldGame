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
	bool m_redrawStaticLighting;

	RenderTarget2D *m_lightingPass0;
	RenderTarget2D *m_lightingPass1;
	RenderTarget2D *m_lightingPass2;
	RenderTarget2D* m_occludersRenderTarget;
	RenderTarget2D* m_shadowMapRenderTarget;
	RenderTarget2D* m_shadowsRenderTarget;

	Resource<Shader> m_shadowMapShader;
	Resource<Shader> m_shadowRenderShader;

	RenderTarget2D *m_blockLightingRenderTarget;
	Resource<Shader> m_directionalLightingShader;
	Resource<Shader> m_radialLightingShader;
	Resource<Shader> m_blurHShader;
	Resource<Shader> m_blurVShader;

	int m_lightMapSize;

	list<LightSource*> m_lightSources;

	void addLightSource(LightSource *lightSource);
	void removeLightSource(LightSource *lightSource);

	void setLightMapResolution(const int size);
	void updateViewSize(int width, int height);

	void redrawStaticLighting(GraphicsContext *context);

	void drawLight(LightSource *light, RenderTarget2D *dest, GraphicsContext *context);
};

#endif // LIGHTING_H