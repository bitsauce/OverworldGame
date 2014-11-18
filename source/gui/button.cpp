#include "button.h"
#include "constants.h"
#include "resources.h"

Button::Button(string text, function<void()> onClick, UiObject *parent) :
	UiObject(parent),
	m_font("Arial Bold", 32),
	m_animTime(0.0f),
	m_userData(0)
{
	// Set callback
	m_callback = onClick;

	// Set sprite
	m_buttonSprite = new XSprite(XTextureRegion());
		
	// Set text
	setText(text);
}
	
/*Button::Button(string text, function<void(Button*)> onClick, UiObject *parent) :
	UiObject(parent),
	m_font(ResourceManager::getFont("font_large")),
	m_animTime(0.0f),
	m_userData(0)
{
	// Set callback
	m_callbackThis = onClick;

	// Set sprite
	m_buttonSprite = new XSprite(XTextureRegion());
		
	// Set text
	setText(text);
}*/

Button::~Button()
{
	delete m_buttonSprite;
}
	
void Button::setText(string text)
{
	// Set text
	m_text = text;
		
	// Store text render texture
	//m_textTexture = font::large->renderToTexture(text);
}

string Button::getText() const
{
	return m_text;
}

void Button::setUserData(void *userData)
{
	m_userData = userData;
}

void *Button::getUserData() const
{
	return m_userData;
}

void Button::update()
{
	if(isHovered())
	{
		m_animTime = min(m_animTime + XGraphics::getTimeStep()*4.0f, 1.0f);
	}
	else
	{
		m_animTime = max(m_animTime - XGraphics::getTimeStep()*4.0f, 0.0f);
	}
	UiObject::update();
}
	
void Button::draw(XBatch *batch)
{
	// Get size and position
	Vector2 position = getPosition();
	Vector2 size = getSize();

	// Draw button sprite
	m_buttonSprite->setSize(size);
	m_buttonSprite->setPosition(position);

	// Draw hovered sprite
	if(m_animTime > 0.0f)
	{
		m_buttonSprite->setColor(Vector4(1.0f, 1.0f, 1.0f, m_animTime));
		m_buttonSprite->setRegion(XTextureRegion(texture::menuButton, 0.0f, 0.0f, 1.0f, 0.5f));
		m_buttonSprite->draw(batch);
	}

	// Draw default sprite
	m_buttonSprite->setColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	m_buttonSprite->setRegion(XTextureRegion(texture::menuButton, 0.0f, 0.5f, 1.0f, 1.0f));
	m_buttonSprite->draw(batch);
		
	// Draw text
	m_font.setColor(Vector4(1.0f));
	m_font.draw(batch, position - (Vector2(font::large->getStringWidth(m_text), font::large->getStringHeight(m_text))-size)*0.5f, m_text);
}
	
void Button::clickEvent()
{
	if(m_callback) {
		m_callback();
	}

	if(m_callbackThis) {
		m_callbackThis(this);
	}
}
