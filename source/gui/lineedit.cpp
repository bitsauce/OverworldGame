#include "lineedit.h"
#include "constants.h"

LineEdit::LineEdit(UiObject *parent) :
	UiObject(parent),
	m_cursorPos(0),
	m_cursorTime(0.0f),
	m_font(xd::ResourceManager::get<XFont>(UI_MENU_BUTTON_FONT))
{
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
	m_cursorTime -= XGraphics::getTimeStep();
	if(m_cursorTime <= 0.0f)
	{
		m_cursorTime = 1.0f;
	}
}

void LineEdit::draw(XBatch *batch)
{
	Vector2 position = getPosition();
	Vector2 size = getSize();
		
	// Apply center alignment
	position.x += (size.x - m_font->getStringWidth(m_text))*0.5f;
		
	m_font->setColor(XColor(255, 255, 255, 255));
	m_font->draw(batch, position, m_text);
	
	if(m_cursorTime >= 0.5f)
	{
		XShape shape(Rect(position.x + m_font->getStringWidth(m_text.substr(0, m_cursorPos)), position.y, 2, m_font->getStringHeight("")));
		shape.setFillColor(XColor(0, 0, 0, 255));
		shape.draw(batch);
	}
}

void LineEdit::insertAt(const uint at, const string &str)
{
	// Check valid index
	if(at > str.size())
		return;
		
	// Insert string at index
	string endStr = str.substr(at);
	m_text = m_text.substr(0, at);
	m_text += m_text + endStr;
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
	
void LineEdit::charEvent(uint code)
{
	// Only add text if active
	//if(!isActive())
	//	return;
		
	switch(code)
	{
	case XD_KEY_BACKSPACE:
		// Remove char behind
		if(m_cursorPos != 0) {
			removeAt(m_cursorPos);
			m_cursorPos--;
		}
		break;
			
	case XD_KEY_ENTER:
		// Call accept function
		if(m_acceptFunc) {
			m_acceptFunc();
		}
		break;
			
	default:
		// Add text
		insertAt(m_cursorPos, ""+(char)code);//formatUtf8(code));
		m_cursorPos++;
		break;
	}
}
	
void LineEdit::keyPressed(XVirtualKey key)
{
	switch(key)
	{
		// Delete
		case XD_KEY_DELETE:
		{
			// Remove char in front
			if(m_cursorPos + 1 <= (int)m_text.size()) {
				removeAt(m_cursorPos + 1);
			}
		}
		break;
			
		// Left cursor key
		case XD_KEY_LEFT:
		{
			// Decrease cursor position
			if(--m_cursorPos < 0) {
				m_cursorPos = 0;
			}
		}
		break;
			
		// Right cursor key
		case XD_KEY_RIGHT:
		{
			if(++m_cursorPos >= (int)m_text.size()) {
				m_cursorPos = m_text.size();
			}
		}
		break;
	}
}
	
void LineEdit::keyReleased(XVirtualKey key)
{
}