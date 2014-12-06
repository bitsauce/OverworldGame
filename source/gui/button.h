#ifndef BUTTON_H
#define BUTTON_H

#include <x2d/x2d.h>
#include "uiobject.h"

class Button : public UiObject
{
public:
	Button(string text, function<void()> onClick, UiObject *parent = nullptr);
	//Button(string text, function<void(Button*)> onClick, UiObject *parent = nullptr);
	~Button();

	void setText(string text);
	string getText() const;

	void setUserData(void *userData);
	void *getUserData() const;

	void update();
	void draw(XBatch *batch);
	void clickEvent();

private:
	// Button text
	string m_text;

	// Button font
	shared_ptr<XFont> m_font;

	// Button sprite texture
	shared_ptr<XTexture> m_btnTexture;
		
	// Button callback
	function<void()> m_callback;
	function<void(Button*)> m_callbackThis;
	
	// User data
	void *m_userData;
	
	// Text texture
	shared_ptr<XTexture> m_textTexture;
		
	// Button sprite
	XSprite *m_buttonSprite;

	// Animation time
	float m_animTime;
};

#endif // BUTTON_H