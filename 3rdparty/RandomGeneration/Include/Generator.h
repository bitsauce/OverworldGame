#pragma once

#include <Sauce/Sauce.h>

using namespace sauce;

class Generator
{
public:
	Resource<Shader> m_noiseShader;
	float m_scale;
	bool m_showNoise;
	Vector2F m_position;
	uint m_seed;
	float m_cliffingDelta;
	Game *m_game;

public:
	Generator(Game *game);

	void draw(DrawEvent *e, RenderTarget2D *rt);
};