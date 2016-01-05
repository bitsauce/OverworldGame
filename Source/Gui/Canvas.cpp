#include "Canvas.h"
#include "Constants.h"

Canvas::Canvas() :
	//UiObject(nullptr),
	m_canvasSize(1, 1),
	m_useWindowSize(true)
{
}

Canvas::Canvas(const uint width, const uint height) :
	//UiObject(nullptr),
	m_canvasSize(width, height),
	m_useWindowSize(false)
{
	//m_anchor.set(0.5f, 0.5f);
}

void Canvas::updateSize()
{
	/*if(m_useWindowSize)
	{
		m_rect.size.set(1.0f, 1.0f);
	}
	else
	{
		Vector2 size;
		if(Window::getSize().x < Window::getSize().y)
		{
			// Fit width and use inverse aspect ratio
			size.x = (float)min(m_canvasSize.x, Window::getSize().x);
			size.y = size.x * (float)m_canvasSize.y/(float)m_canvasSize.x; 
		}
		else
		{
			// Fit height and use aspect ratio
			size.y = (float)min(m_canvasSize.y, Window::getSize().y);
			size.x = size.y * (float)m_canvasSize.x/(float)m_canvasSize.y; 
		}
		m_rect.size = size/Vector2(Window::getSize());
	}
	//Debug::setVaraible("Canvas", "("+util::floatToStr(size.x)+", "+util::floatToStr(size.y)+")");*/
}

void Canvas::onTick(TickEvent *e)
{
	updateSize();
	UiObject::onTick(e);
}