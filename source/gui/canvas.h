#ifndef CANVAS_H
#define CANVAS_H

#include "Config.h"
#include "UiObject.h"

class Canvas : public UiObject
{
public:
	Canvas(const uint w, const uint h);

	Vector2i getCanvasSize() const { return m_canvasSize; }

	void update();

private:
	Vector2i m_canvasSize;
};

#endif // CANVAS_H