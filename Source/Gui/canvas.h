#ifndef CANVAS_H
#define CANVAS_H

#include "UiObject.h"

class Canvas : public UiObject
{
public:
	Canvas(Window *window);
	Canvas(Window *window, const int width, const int height);

	void onWindowSizeChanged(WindowEvent *e);

	Vector2I getDrawPosition();
	Vector2I getDrawSize();

private:
	Window *m_window;
	const int m_canvasWidth, m_canvasHeight;
};

#endif // CANVAS_H