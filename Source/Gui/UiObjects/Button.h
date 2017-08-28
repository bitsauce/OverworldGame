#ifndef BUTTON_H
#define BUTTON_H

#include "UiObject.h"

class Button : public UiObject
{
public:
	Button(UiObject *parent, const uint btnWidth, const uint btnHeight);

	void onClick(ClickEvent *e);
	void onDraw(DrawEvent *e);

	void setText(const string &text) { m_text = text; }
	void setOnClickCallback(function<void()> callback);

private:
	Resource<Texture2D> m_texture, m_textureHover, m_textureActive;
	Resource<Font> m_font;
	string m_text;
	RenderTarget2D *m_renderTarget;
	SpriteBatch m_spriteBatch;
	function<void()> m_clickCallback;
};

#endif // BUTTON_H