#ifndef LIGHTING_H
#define LIGHTING_H

#include "Config.h"
#include "LightSource.h"
#include "Spotlight.h"

class Terrain;
class World;

class Lighting : public WindowListener
{
	friend class Lighting;
	friend class Debug;
public:
	// CONSTRUCTOR
	Lighting(World *world);
	~Lighting();

	// DRAWING
	//xd::RenderTarget2D *drawLightMaps();
	void draw(xd::GraphicsContext &gfxContext);

	// WINDOW
	void resizeEvent(uint width, uint height);
	
	void addLightSource(LightSource *lightSource);

	void draw(xd::SpriteBatch *spriteBatch);

private:
	Terrain *m_terrain;
	World *m_world;

	uint m_width, m_height;
	RenderTarget2D *m_lightingRenderTarget;
	RenderTarget2D *m_lightingPass0;
	RenderTarget2D *m_lightingPass1;
	RenderTarget2D *m_lightingPass2;
	ShaderPtr m_directionalLightingShader;
	ShaderPtr m_radialLightingShader;
	ShaderPtr m_blurHShader;
	ShaderPtr m_blurVShader;
	int m_lightRadius;
	bool m_enabled;

	list<LightSource*> m_lightSources;
};

#endif // LIGHTING_H