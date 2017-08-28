#pragma once

#include "Config.h"
#include "UiObject.h"

class ColorPicker : public UiObject
{
public:
	ColorPicker(UiObject *parent);

	void setSelectedColor(const Color &color);
	Color getSelectedColor() const;

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	void onResize(ResizeEvent *e);
	void onMouseEvent(MouseEvent *e);

private:
	RenderTarget2D *m_renderTarget;
	map<Vector2I, ColorRGB> m_coordToColorMap;
	map<ColorRGB, Vector2I> m_colorToCoordMap;
	bool m_leftMouseButton;
	Resource<Texture2D> m_colorWheelTexture;
	Sprite m_ringSprite;
	Vector2F m_ringLocation;
	ColorRGB m_selectedColor;
	uchar m_selectedAlpha;
	bool m_renderColorWheel;
};