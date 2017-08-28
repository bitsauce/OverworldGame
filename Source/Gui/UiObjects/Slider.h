#pragma once

#include "UiObject.h"

class Slider : public UiObject
{
public:
	Slider(UiObject *parent);

	void onDraw(DrawEvent *e);
	void onMouseEvent(MouseEvent *e);

private:
	Resource<Texture2D> m_texture;
	float m_percentage;
};