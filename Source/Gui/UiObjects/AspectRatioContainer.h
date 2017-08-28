#ifndef ASPECT_RATIO_CONTAINER_H
#define ASPECT_RATIO_CONTAINER_H

#include "UiObject.h"

class AspectRatioContainer : public UiObject
{
public:
	AspectRatioContainer(UiObject *parent, Window *window, const int maxWidth, const float aspectRatio);

	void onResize(ResizeEvent *e);
	void onDraw(DrawEvent *e);

private:
	Window *m_window;
	const int m_maxWidth;
	const float m_aspectRatio;
};

#endif // ASPECT_RATIO_CONTAINER_H