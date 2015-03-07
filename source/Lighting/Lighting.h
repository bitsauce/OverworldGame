#ifndef LIGHTING_H
#define LIGHTING_H

#include "Config.h"
#include "Game/GameObject.h"

class Terrain;
class LightSource;
class World;

class Lighting : public GameObject, public WindowListener
{
	friend class Lighting;
	friend class Debug;
public:
	// CONSTRUCTOR
	Lighting(World &world);
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
	World &m_world;

	uint m_width, m_height;
	xd::RenderTarget2D *m_lightingRenderTarget;
	xd::RenderTarget2D *m_lightingPass0;
	xd::RenderTarget2D *m_lightingPass1;
	xd::RenderTarget2D *m_lightingPass2;
	xd::ShaderPtr m_directionalLightingShader;
	xd::ShaderPtr m_radialLightingShader;
	xd::ShaderPtr m_blurHShader;
	xd::ShaderPtr m_blurVShader;
	int m_lightRadius;
	bool m_enabled;

	list<LightSource*> m_lightSources;
};

#endif // LIGHTING_H