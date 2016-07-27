#pragma once

#include "Config.h"
#include "UiObject.h"

class ColorPicker : public UiObject
{
public:
	ColorPicker(UiObject *parent);

	Color getSelectedColor() const;

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	void onResize(ResizeEvent *e);
	void onMouseEvent(MouseEvent *e);

private:
	Resource<Shader> m_colorWheelShader;
	RenderTarget2D *m_renderTarget;

	bool m_leftMouseButton;
	Sprite m_ringSprite;
	Vector2F m_ringLocation;
	Color m_selectedColor;
};