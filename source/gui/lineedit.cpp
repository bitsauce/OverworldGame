#include "lineedit.h"
#include "constants.h"

LineEdit::LineEdit(UiObject *parent) :
	UiObject(parent),
	m_cursorPos(0),
	m_cursorTime(0.0f),
	m_font(xd::ResourceManager::get<xd::Font>(UI_MENU_BUTTON_FONT))
{
	xd::Input::addKeyboardListener(this);
}

void LineEdit::setText(const string &text)
{
	m_text = text;
}

string LineEdit::getText() const
{
	return m_text;
}

void LineEdit::update()
{
	m_cursorTime -= xd::Graphics::getTimeStep();
	if(m_cursorTime <= 0.0f)
	{
		m_cursorTime = 1.0f;
	}
}

void LineEdit::draw(xd::SpriteBatch *spriteBatch)
{
	Vector2 position = getPosition();
	Vector2 size = getSize();
		
	// Apply center alignment
	position.x += (size.x - m_font->getStringWidth(m_text))*0.5f;
		
	m_font->setColor(xd::Color(255, 255, 255, 255));
	m_font->draw(spriteBatch, position, m_text);
	
	xd::GraphicsContext &gfxContext = spriteBatch->getGraphicsContext();
	if(m_cursorTime >= 0.5f)
	{
		gfxContext.drawRectangle(position.x + m_font->getStringWidth(m_text.substr(0, m_cursorPos)), position.y, 2, m_font->getStringHeight(""), xd::Color(0, 0, 0, 255));
	}
}

void LineEdit::insertAt(const uint at, const string &str)
{
	// Check valid index
	if(at > m_text.size())
		return;
		
	// Insert string at index
	string endStr = m_text.substr(at);
	m_text = m_text.substr(0, at);
	m_text += str + endStr;
}

void LineEdit::removeAt(const uint at)
{
	// Check valid index
	if(at > m_text.size())
		return;
		
	// Remove char at index
	string endStr = m_text.substr(at);
	m_text = m_text.substr(0, at-1);
	m_text += endStr;
}
	
void LineEdit::charEvent(const wchar_t c)
{
	// Only add text if active
	//if(!isActive())
	//	return;

		
	switch(c)
	{
	case xd::XD_KEY_BACKSPACE:
		// Remove char behind
		if(m_cursorPos != 0) {
			removeAt(m_cursorPos);
			m_cursorPos--;
		}
		break;
			
	case xd::XD_KEY_ENTER:
		// Call accept function
		if(m_acceptFunc) {
			m_acceptFunc();
		}
		break;
			
	default:
		// Add text
		insertAt(m_cursorPos, string("")+(char)c);//formatUtf8(code));
		m_cursorPos++;
		break;
	}
}
	
void LineEdit::keyPressEvent(const xd::VirtualKey key)
{
	switch(key)
	{
		// Delete
		case xd::XD_KEY_DELETE:
		{
			// Remove char in front
			if(m_cursorPos + 1 <= (int)m_text.size()) {
				removeAt(m_cursorPos + 1);
			}
		}
		break;
			
		// Left cursor key
		case xd::XD_KEY_LEFT:
		{
			// Decrease cursor position
			if(--m_cursorPos < 0) {
				m_cursorPos = 0;
			}
		}
		break;
			
		// Right cursor key
		case xd::XD_KEY_RIGHT:
		{
			if(++m_cursorPos >= (int)m_text.size()) {
				m_cursorPos = m_text.size();
			}
		}
		break;
	}
}
	
void LineEdit::keyReleaseEvent(const xd::VirtualKey key)
{
}