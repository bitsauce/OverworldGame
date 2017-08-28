#ifndef LABEL_H
#define LABEL_H

#include "UiObject.h"

class Label : public UiObject
{
public:
	Label(UiObject *parent);

	void onDraw(DrawEvent *e);

	void setText(const string &text)
	{
		m_text = text;
	}

private:
	Resource<Font> m_font;
	string m_text;
	SpriteBatch m_spriteBatch;
};

#endif // LABEL_H