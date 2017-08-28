#include "AspectRatioContainer.h"

AspectRatioContainer::AspectRatioContainer(UiObject *parent, Window *window, const int maxWidth, const float aspectRatio) :
	UiObject(parent),
	m_window(window),
	m_maxWidth(maxWidth),
	m_aspectRatio(aspectRatio)
{
	setAnchor(0.5f, 0.5f);
	setOrigin(0.5f, 0.5f);
	setSize(1.0f, 1.0f);
}

void AspectRatioContainer::onResize(ResizeEvent *e)
{
	Vector2I parentSize = getParent()->getDrawSize();

	// NOTE: There might be some simplification potential here
	Vector2F size;
	size.x = min(m_aspectRatio / ((float) parentSize.x / (float) parentSize.y), min((float) m_maxWidth / (float) parentSize.x, 1.0f));
	size.y = min(((float) parentSize.x / (float) parentSize.y) / m_aspectRatio, min((float) (m_maxWidth / m_aspectRatio) / (float) parentSize.y, 1.0f));
	setSize(size);
	UiObject::onResize(e);
}

void AspectRatioContainer::onDraw(DrawEvent *e)
{
	UiObject::onDraw(e);

	//if(debug) {
		e->getGraphicsContext()->drawRectangleOutline(getDrawRect(), Color(127, 127, 127));
	//}
}