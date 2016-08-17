#include "Generator.h"

Generator::Generator(Game *game) :
	m_game(game),
	m_scale(1.0f),
	m_showNoise(false),
	m_seed(0),
	m_cliffingDelta(70.0f)
{
	m_noiseShader = m_Resource<Shader>("Generation");
}

void Generator::draw(DrawEvent *e, RenderTarget2D *rt)
{
	GraphicsContext *context = e->getGraphicsContext();

	// Set uniforms
	m_noiseShader->setUniform1i("u_ShowNoise", m_showNoise);
	m_noiseShader->setUniform2f("u_Resolution", rt->getWidth(), rt->getHeight());
	m_noiseShader->setUniform1f("u_Scale", m_scale);
	m_noiseShader->setUniform2f("u_Position", m_position.x, m_position.y);
	m_noiseShader->setUniform1ui("u_Seed", m_seed);
	m_noiseShader->setUniform1f("u_CliffingDelta", m_cliffingDelta);

	// Render first pass to render-target
	context->setBlendState(BlendState(BlendState::BLEND_ONE, BlendState::BLEND_ZERO));
	context->setRenderTarget(rt);
	context->setShader(m_noiseShader);
	context->drawRectangle(0, 0, rt->getWidth(), rt->getHeight());
	context->setShader(0);
	context->setRenderTarget(0);
	context->setBlendState(BlendState(BlendState::PRESET_ALPHA_BLEND));
}