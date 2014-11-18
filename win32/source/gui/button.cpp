#include "button.h"
#include "constants.h"

Button::Button(string text, delegate<void()> *onClick, UiObject *parent) :
	UiObject(parent),
	animTime(0.0f)
{
	// Set callback
	if(onClick) {
		this->callback = *onClick;
	}

	// Set sprite
	this->buttonSprite = new XSprite(XTextureRegion());
		
	// Set text
	setText(text);
}
	
/*Button(string text, ButtonCallbackThis @callbackThis, UiObject @parent)
{
	// Call parent constructor
	super(@parent);
		
	// Set callback
	@this.callbackThis = @callbackThis;
		
	// Set text
	setText(text);
}*/
	
void Button::setText(string text)
{
	// Set text
	this->text = text;
		
	// Store text render texture
	textTexture = font::large->renderToTexture(text);
}
	
void Button::update()
{
	if(isHovered())
	{
		animTime = min(animTime + XGraphics::getTimeStep()*4.0f, 1.0f);
	}
	else
	{
		animTime = max(animTime - XGraphics::getTimeStep()*4.0f, 0.0f);
	}
	UiObject::update();
}
	
void Button::draw(XBatch *batch)
{
	// Get size and position
	Vector2 position = getPosition();
	Vector2 size = getSize();

	// Draw button sprite
	buttonSprite->setSize(size);
	buttonSprite->setPosition(position);

	// Draw hovered sprite
	if(animTime > 0.0f)
	{
		buttonSprite->setColor(Vector4(1.0f, 1.0f, 1.0f, animTime));
		buttonSprite->setRegion(XTextureRegion(texture::menuButton, 0.0f, 0.0f, 1.0f, 0.5f));
		buttonSprite->draw(batch);
	}

	// Draw default sprite
	buttonSprite->setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	buttonSprite->setRegion(XTextureRegion(texture::menuButton, 0.0f, 0.5f, 1.0f, 1.0f));
	buttonSprite->draw(batch);
		
	// Draw text
	font::large->setColor(Vector4(1.0f));
	font::large->draw(batch, position - (Vector2(font::large->getStringWidth(text), font::large->getStringHeight(text))-size)*0.5f, text);
}
	
void Button::clickEvent()
{
	// Call click callbacks
	if(callback != nullptr) {
		callback();
	}
		
	/*if(callbackThis != null) {
		callbackThis(@this);
	}*/
}
