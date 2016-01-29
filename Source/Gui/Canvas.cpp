#include "Canvas.h"

Canvas::Canvas(Window *window) :
	UiObject(0),
	m_window(window),
	m_canvasWidth(0),
	m_canvasHeight(0)
{
	WindowEvent e(WindowEvent::SIZE_CHANGED, window, window->getWidth(), window->getHeight());
	onWindowSizeChanged(&e);
}

Canvas::Canvas(Window *window, const int width, const int height) :
	UiObject(0),
	m_window(window),
	m_canvasWidth(width),
	m_canvasHeight(height)
{
	setAnchor(0.5f, 0.5f);
	setOrigin(0.5f, 0.5f);

	WindowEvent e(WindowEvent::SIZE_CHANGED, window, window->getWidth(), window->getHeight());
	onWindowSizeChanged(&e);
}

void Canvas::onWindowSizeChanged(WindowEvent *e)
{
	Vector2F size;
	if(m_canvasWidth <= 0 || m_canvasHeight <= 0)
	{
		size.set(e->getWidth(), e->getHeight());
	}
	else if(e->getWidth() > e->getHeight())
	{
		// Fit width and use inverse aspect ratio
		size.x = (float) min(m_canvasWidth, e->getWidth());
		size.y = size.x * (float) m_canvasHeight / (float) m_canvasWidth;
	}
	else
	{
		// Fit height and use aspect ratio
		size.y = (float) min(m_canvasHeight, e->getHeight());
		size.x = size.y * (float) m_canvasWidth / (float) m_canvasHeight;
	}
	setSize(size);
	UiObject::onWindowSizeChanged(e);
}

Vector2I Canvas::getDrawPosition()
{
	Vector2F parentPos = Vector2F();
	Vector2F parentSize = m_window->getSize();
	Vector2F pos = Vector2F();
	Vector2F size = getSize();

	parentPos += parentSize * Vector2F(0.5f, 0.5f);
	pos -= size * Vector2F(0.5f, 0.5f);
	return parentPos + pos;
}

Vector2I Canvas::getDrawSize()
{
	return getSize();
}
