#include "Canvas.h"

Canvas::Canvas(Window *window, const bool transparent) :
	UiObject(0, false)
{
	setSize(window->getWidth(), window->getHeight());
}

void Canvas::onWindowSizeChanged(WindowEvent *e)
{
	setSize(e->getWidth(), e->getHeight());
}