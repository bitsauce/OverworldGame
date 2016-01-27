#ifndef CANVAS_H
#define CANVAS_H

#include "Config.h"
#include "UiObject.h"

class Canvas : public UiObject
{
public:
	Canvas();
	Canvas(const uint width, const uint height);

	Vector2I getCanvasSize() const { return m_canvasSize; }

	void updateSize();
	void onTick(TickEvent *e);

private:
	Vector2I m_canvasSize;
	bool m_useWindowSize;
};

#endif // CANVAS_H