#ifndef BLOCK_LIGHTING_H
#define BLOCK_LIGHTING_H

#include <x2d/x2d.h>

class Terrain;

class TerrainLighting : public GameObject, public xd::WindowListener
{
public:
	// CONSTRUCTOR
	TerrainLighting(Terrain *terrain);
	~TerrainLighting();

	// DRAWING
	void draw(xd::SpriteBatch *spriteBatch);

	// WINDOW
	void resizeEvent(uint width, uint height);

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
};

#endif // BLOCK_LIGHTING_H