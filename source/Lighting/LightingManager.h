#ifndef LIGHTING_MANAGER_H
#define LIGHTING_MANAGER_H

#include <x2d/x2d.h>

class Terrain;
class LightSource;

class LightingManager : public GameObject, public xd::WindowListener
{
	friend class LightingManager;
	friend class Debug;
public:
	// CONSTRUCTOR
	LightingManager(Terrain *terrain);
	~LightingManager();

	// DRAWING
	//xd::RenderTarget2D *drawLightMaps();
	void draw(xd::GraphicsContext &gfxContext);

	// WINDOW
	void resizeEvent(uint width, uint height);
	
	void addLightSource(LightSource *lightSource);

	void draw(xd::SpriteBatch *spriteBatch);

private:
	Terrain *m_terrain;
	uint m_width, m_height;
	xd::RenderTarget2D *m_lightingRenderTarget;
	xd::RenderTarget2D *m_lightingPass0;
	xd::RenderTarget2D *m_lightingPass1;
	xd::RenderTarget2D *m_lightingPass2;
	xd::ShaderPtr m_directionalLightingShader;
	xd::ShaderPtr m_blurHShader;
	xd::ShaderPtr m_blurVShader;
	int m_lightRadius;

	// RADIAL SHADER
	xd::ShaderPtr m_radialLightingShader;

	list<LightSource*> m_lightSources;
};

#endif // LIGHTING_MANAGER_H