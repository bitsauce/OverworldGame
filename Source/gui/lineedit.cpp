#include "lineedit.h"
#include "constants.h"
#include "Game/Scene.h"

LineEdit::LineEdit(Scene *scene, UiObject *parent) :
	UiObject(scene, parent),
	m_cursorPos(0),
	m_cursorTime(0.0f),
	m_font(ResourceManager::get<Font>(UI_CHAT_FONT))
{
	m_font->setColor(Color(255, 255, 255, 255));
}

LineEdit::~LineEdit()
{
}

void LineEdit::setText(const string &text)
{
	m_text = text;
	m_cursorPos = m_text.size();
}

string LineEdit::getText() const
{
	return m_text;
}

void LineEdit::update(const float delta)
{
	m_cursorTime -= delta;
	if(m_cursorTime <= 0.0f)
	{
		m_cursorTime = 1.0f;
	}
	UiObject::update(delta);
}

void LineEdit::draw(SpriteBatch *spriteBatch)
{
	Vector2 position = getPosition();
	Vector2 size = getSize();

	GraphicsContext &context = spriteBatch->getGraphicsContext();
	context.setModelViewMatrix(Matrix4());
	context.drawRectangle(position, size, Color(0, 0, 0, 127));
		
	// Apply center alignment
	//position.x += (size.x - m_font->getStringWidth(m_text)) * 0.5f;
	
	position += Vector2(5.0f);

	m_font->draw(spriteBatch, position, m_text);
	
	if(m_active && m_cursorTime >= 0.5f)
	{
		context.drawRectangle(position.x + m_font->getStringWidth(m_text.substr(0, m_cursorPos)), position.y + 2, 2, m_font->getHeight() - 4, m_font->getColor());
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
	
void LineEdit::charEvent(const uint c)
{
	// Only add text if active
	if(!m_active) return;
		
	switch(c)
	{
			
	default:
		// Add text
		insertAt(m_cursorPos, string("")+(char)c);//formatUtf8(code));
		m_cursorPos++;
		break;
	}
}
	
void LineEdit::keyPressEvent(const VirtualKey key)
{
	// Only add text if active
	if(!m_active) return;

	switch(key)
	{
		// Enter
		case XD_KEY_ENTER:
		{
			// Call accept function
			if(m_acceptFunc)
			{
				m_acceptFunc();
			}
			break;
		}

		// Backspace
		case XD_KEY_BACKSPACE:
		{
			// Remove char behind
			if(m_cursorPos != 0)
			{
				removeAt(m_cursorPos);
				m_cursorPos--;
			}
			break;
		}

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
	
void LineEdit::keyReleaseEvent(const VirtualKey key)
{
}