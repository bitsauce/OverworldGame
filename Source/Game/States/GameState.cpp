#include "GameState.h"
#include "Gui/Gui.h"

GameState::GameState(const GameStateID id, const bool transparent) :
	m_id(id),
	m_transparent(transparent),
	m_renderTarget(0)
{
	Window *window = Game::Get()->getWindow();
	m_renderTarget = new RenderTarget2D(window->getWidth(), window->getHeight());
}

void GameState::onDraw(DrawEvent *e)
{
	GraphicsContext *graphicsContext = e->getGraphicsContext();
	//graphicsContext->pushState();
	//graphicsContext->setRenderTarget(m_renderTarget);
	//graphicsContext->setBlendState(BlendState(BlendState::BLEND_ZERO, BlendState::BLEND_ZERO));
	//graphicsContext->drawRectangle(0, 0, m_renderTarget->getWidth(), m_renderTarget->getHeight());
	//graphicsContext->setBlendState(BlendState(BlendState::PRESET_PREMULTIPLIED_ALPHA));
	SceneObject::onDraw(e);
	//graphicsContext->popState();
}

void GameState::onWindowSizeChanged(WindowEvent *e)
{
	if(m_renderTarget->getWidth() != e->getWidth() || m_renderTarget->getHeight() != e->getHeight())
	{
		delete m_renderTarget;
		m_renderTarget = new RenderTarget2D(e->getWidth(), e->getHeight());
	}
}