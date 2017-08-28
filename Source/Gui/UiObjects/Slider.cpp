#include "Slider.h"

// TODO: When clicking the circle, it should drag even if the click is outside the box

Slider::Slider(UiObject *parent) :
	UiObject(parent),
	m_percentage(0.0f),
	m_texture(Resource<Texture2D>("Slider"))
{
}

void Slider::onMouseEvent(MouseEvent *e)
{
	UiObject::onMouseEvent(e);

	if(isPressed())
	{
		m_percentage = math::clamp(float(e->getX() - getDrawPosition().x) / (getDrawSize().x - 8.0f), 0.0f, 1.0f);
	}
}

void Slider::onDraw(DrawEvent *e)
{
	RectI rect = getDrawRect();

	GraphicsContext *graphicsContext = e->getGraphicsContext();

	const float w = m_texture->getWidth(), h = m_texture->getHeight();

	graphicsContext->setTexture(m_texture);

	// Dark part of slider
	graphicsContext->drawRectangle(rect.position.x, rect.position.y + (rect.size.y - 8.0f) * 0.5f, 4.0f, 8.0f, Color::White, TextureRegion(0.0f, 16.0f / h, 4.0f / w, 24.0f / h));
	graphicsContext->drawRectangle(rect.position.x + 4.0f, rect.position.y + (rect.size.y - 8.0f) * 0.5f, (rect.size.x - 4.0f) * m_percentage, 8.0f, Color::White, TextureRegion(4.0f / w, 16.0f / h, 8.0f / w, 24.0f / h));

	// Bright part of slider
	graphicsContext->drawRectangle(rect.position.x + (rect.size.x - 4.0f) * m_percentage, rect.position.y + (rect.size.y - 8.0f) * 0.5f, rect.size.x - 4.0f - (rect.size.x - 4.0f) * m_percentage, 8.0f, Color::White, TextureRegion(4.0f / w, 0.0f, 8.0f / w, 8.0f / h));
	graphicsContext->drawRectangle(rect.position.x + rect.size.x - 4.0f, rect.position.y + (rect.size.y - 8.0f) * 0.5f, 4.0f, 8.0f, Color::White, TextureRegion(8.0f / w, 0.0f, 12.0f / w, 8.0f / h));
	
	// Slider circle
	graphicsContext->drawRectangle(rect.position.x - 13.0f + (rect.size.x - 4.0f) * m_percentage, rect.position.y + (rect.size.y - 26.0f) * 0.5f, 26.0f, 26.0f, Color(255, 255, 255, 127), TextureRegion(16.0f / w, 0.0f, 1.0f, 1.0f));
	graphicsContext->setTexture(0);
}
