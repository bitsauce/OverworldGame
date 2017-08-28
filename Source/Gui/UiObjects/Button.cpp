#include "Button.h"

Button::Button(UiObject *parent, const uint btnWidth, const uint btnHeight) :
	UiObject(parent),
	m_texture(Resource<Texture2D>("Gui/ButtonInactive")),
	m_textureHover(Resource<Texture2D>("Gui/ButtonHover")),
	m_textureActive(Resource<Texture2D>("Gui/ButtonActive")),
	m_font(Resource<Font>("Fonts/MenuFont")),
	m_spriteBatch(100)
{
	m_renderTarget = new RenderTarget2D(btnWidth, btnHeight);
}

void Button::onClick(ClickEvent *e)
{
	switch(e->getType())
	{
		case ClickEvent::DONE:
			if(m_clickCallback)
			{
				m_clickCallback();
			}
			break;
	}

	UiObject::onClick(e);
}

void Button::onDraw(DrawEvent *e)
{
	GraphicsContext *graphicsContext = e->getGraphicsContext();

	// Draw button to render target
	graphicsContext->pushState();
	graphicsContext->setRenderTarget(m_renderTarget);
	graphicsContext->setBlendState(BlendState(BlendState::BLEND_SRC_ALPHA, BlendState::BLEND_ZERO, BlendState::BLEND_ONE, BlendState::BLEND_ZERO));

	if(isPressed() && isHovered())
	{
		graphicsContext->setTexture(m_textureActive);
	}
	else if(isHovered())
	{
		graphicsContext->setTexture(m_textureHover);
	}
	else
	{
		graphicsContext->setTexture(m_texture);
	}

	const Vector2I size(m_renderTarget->getWidth(), m_renderTarget->getHeight());
	
	graphicsContext->drawRectangle(0.0f,           0.0f,           16.0f, 16.0f, Color::White, TextureRegion(0.0f, 0.0f, 1.0f / 3.0f, 1.0f / 3.0f));
	graphicsContext->drawRectangle(size.x - 16.0f, 0.0f,           16.0f, 16.0f, Color::White, TextureRegion(2.0f / 3.0f, 0.0f, 1.0f, 1.0f / 3.0f));
	graphicsContext->drawRectangle(0.0f,           size.y - 16.0f, 16.0f, 16.0f, Color::White, TextureRegion(0.0f, 2.0f / 3.0f, 1.0f / 3.0f, 1.0f));
	graphicsContext->drawRectangle(size.x - 16.0f, size.y - 16.0f, 16.0f, 16.0f, Color::White, TextureRegion(2.0f / 3.0f, 2.0f / 3.0f, 1.0f, 1.0f));

	graphicsContext->drawRectangle(16.0f,          0.0f,           size.x - 32.0f, 16.0f,          Color::White, TextureRegion(1.0f / 3.0f, 0.0f / 3.0f, 2.0f / 3.0f, 1.0f / 3.0f));
	graphicsContext->drawRectangle(16.0f,          size.y - 16.0f, size.x - 32.0f, 16.0f,          Color::White, TextureRegion(1.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));
	graphicsContext->drawRectangle(0.0f,           16.0f,          16.0f,          size.y - 32.0f, Color::White, TextureRegion(0.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f));
	graphicsContext->drawRectangle(size.x - 16.0f, 16.0f,          16.0f,          size.y - 32.0f, Color::White, TextureRegion(2.0f / 3.0f, 1.0f / 3.0f, 3.0f / 3.0f, 2.0f / 3.0f));

	graphicsContext->drawRectangle(Vector2F(16.0f), size - Vector2F(32.0f), Color::White, TextureRegion(1.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f));
	graphicsContext->popState();

	// Draw button from render target
	RectI rect = getDrawRect();
	graphicsContext->setTexture(m_renderTarget->getTexture());
	graphicsContext->drawRectangle(rect);
	graphicsContext->setTexture(0);

	m_spriteBatch.begin(graphicsContext, SpriteBatch::State(SpriteBatch::DEFERRED, BlendState(BlendState::BLEND_SRC_ALPHA, BlendState::BLEND_ONE_MINUS_SRC_ALPHA, BlendState::BLEND_ONE, BlendState::BLEND_ONE_MINUS_SRC_ALPHA)));
	m_font->setHeight(min(float(rect.size.y), 17.0f));
	m_font->setColor(Color(0, 0, 0, 255));
	m_font->draw(&m_spriteBatch, Vector2I(rect.position + rect.size / 2 - Vector2F(0.0f, m_font->getHeight() * 0.5f)), m_text, FONT_ALIGN_CENTER);
	m_font->setHeight(17.0f);
	m_spriteBatch.end();

	UiObject::onDraw(e);
}

void Button::setOnClickCallback(function<void()> callback)
{
	m_clickCallback = callback;
}
