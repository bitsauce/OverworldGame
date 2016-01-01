#ifndef BUTTON_H
#define BUTTON_H

#include "Config.h"
#include "UiObject.h"

class Button : public UiObject
{
public:
	Button(Scene *scene, string text, function<void()> onClick, UiObject *parent = nullptr);
	//Button(string text, function<void(Button*)> onClick, UiObject *parent = nullptr);
	~Button();

	void setText(string text);
	string getText() const;

	void setUserData(void *userData);
	void *getUserData() const;

	void onTick(TickEvent *e);
	void draw(SpriteBatch *SpriteBatch);
	void clickEvent();

private:
	// Button text
	string m_text;

	// Button font
	FontPtr m_font;

	// Button sprite texture
	Texture2DPtr m_btnTexture;
		
	// Button callback
	function<void()> m_callback;
	function<void(Button*)> m_callbackThis;
	
	// User data
	void *m_userData;
	
	// Text texture
	Texture2DPtr m_textTexture;
		
	// Button sprite
	Sprite *m_buttonSprite;

	// SkeletalAnimation time
	float m_animTime;
};

#endif // BUTTON_H