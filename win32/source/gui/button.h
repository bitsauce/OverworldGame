#ifndef BUTTON_H
#define BUTTON_H

#include <x2d/x2d.h>
#include "uiobject.h"

class Button : public UiObject
{
public:
	Button(string text, delegate<void()> *onClick, UiObject *parent = nullptr);
	void setText(string text);
	void update();
	void draw(XBatch *batch);
	void clickEvent();

private:
	// Button text
	string text;
		
	// Button callback
	delegate<void()> callback;
	//delegate<void()> callbackThis;
		
	// User data
	void *userData;
	
	// Text texture
	XTexture *textTexture;
		
	// Button sprite
	XSprite *buttonSprite;

	// Animation time
	float animTime;
};

#endif // BUTTON_H