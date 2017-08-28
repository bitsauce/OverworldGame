#pragma once

#include "UiObject.h"

class GradientBackground : public UiObject
{
public:
	GradientBackground(UiObject *parent);

	void onDraw(DrawEvent *e);

private:
	Color m_topColor, m_bottomColor;
	Vertex m_vertices[4];
};