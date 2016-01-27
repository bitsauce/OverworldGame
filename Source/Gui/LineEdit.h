#ifndef LINE_EDIT_H
#define LINE_EDIT_H

#include "Config.h"
#include "UiObject.h"

class UiObject;

class LineEdit : public UiObject
{
public:
	LineEdit(Scene *scene, UiObject *parent = nullptr);
	~LineEdit();

	void setAcceptFunc(function<void()> func) { m_acceptFunc = func ; }

	void setText(const string &text);
	string getText() const;

	void setColor(const Color &color)
	{
		m_font->setColor(color);
	}

	void onTick(TickEvent *e);
	void draw(SpriteBatch *SpriteBatch);

protected:
	void insertAt(const uint at, const string &str);
	void removeAt(const uint at);
	void charEvent(const uint code);
	void keyEvent(const KeyEvent & event);

	Resource<Font> m_font;
	Color m_color;
	string m_text;
	int m_cursorPos;
	float m_cursorTime;
	function<void()> m_acceptFunc;
};

#endif // LINE_EDIT_H