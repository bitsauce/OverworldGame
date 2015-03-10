#ifndef CANVAS_H
#define CANVAS_H

#include "Config.h"
#include "UiObject.h"

class Canvas : public UiObject
{
public:
	Canvas();
	Canvas(const uint w, const uint h);

	Vector2i getCanvasSize() const { return m_canvasSize; }

	void updateSize();
	void update(const float dt);

private:
	Vector2i m_canvasSize;
	bool m_useWindowSize;
};

#endif // CANVAS_H