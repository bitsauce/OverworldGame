#ifndef LINE_EDIT_H
#define LINE_EDIT_H

#include "Config.h"
#include "UiObject.h"

class UiObject;
class Scene;

class LineEdit : public UiObject, public KeyListener
{
public:
	LineEdit(Scene *scene, UiObject *parent = nullptr);
	~LineEdit();

	void setAcceptFunc(function<void()> func) { m_acceptFunc = func ; }

	void setText(const string &text);
	string getText() const;

	void update(const float delta);
	void draw(SpriteBatch *SpriteBatch);

private:
	void insertAt(const uint at, const string &str);
	void removeAt(const uint at);
	void charEvent(const wchar_t code);
	void keyPressEvent(const VirtualKey key);
	void keyReleaseEvent(const VirtualKey key);

	FontPtr m_font;
	string m_text;
	int m_cursorPos;
	float m_cursorTime;
	function<void()> m_acceptFunc;
};

#endif // LINE_EDIT_H