#include "LineEdit.h"

LineEdit::LineEdit(GraphicsContext *gfx, UiObject *parent) :
	UiObject(parent),
	m_cursorTime(0.0f),
	m_offsetX(0.0f),
	m_font(Resource<Font>("Fonts/Chat")),
	m_renderTarget(0),
	m_dirty(true),
	m_spriteBatch(gfx, 100),
	m_wordBegin(0),
	m_wordEnd(0)
{
	setText("");
}

LineEdit::~LineEdit()
{
}

void LineEdit::setAcceptFunc(function<void()> func)
{
	m_acceptFunc = func;
}

void LineEdit::setText(const string &text)
{
	m_states.clear();

	for(int i = 0; i < 2; ++i)
	{
		TextState *state = new TextState { Cursor(this),  text };
		m_states.push_back(state);
		m_undoItr = m_states.end();
		m_undoItr--;
		state->cursor.setPosition(text.size());
	}

	m_dirty = true;
}

string LineEdit::getText() const
{
	return (*m_undoItr)->text;
}

void LineEdit::onTick(TickEvent *e)
{
	// Update cursor blink time
	m_cursorTime -= e->getDelta();
	if(m_cursorTime <= 0.0f)
	{
		m_cursorTime = 1.0f;
	}
	UiObject::onTick(e);
}

void LineEdit::onDraw(DrawEvent *e)
{
	// Get state variables
	TextState *state = *m_undoItr;
	RectI rect = getDrawRect();
	GraphicsContext *g = e->getGraphicsContext();

	// Update text offset if dirty
	if(m_dirty)
	{
		updateOffset();
	}

	// Get text offset
	Vector2I textOffset(8.0f - m_offsetX, rect.size.y * 0.5f - m_font->getHeight() * 0.5f);

	// Update visualization if dirty
	if(m_dirty)
	{
		// Update line edit visualization
		g->setRenderTarget(m_renderTarget);
		const float w = m_renderTarget->getWidth(), h = m_renderTarget->getHeight();
		g->setTexture(0);

		// Clear render target
		g->setBlendState(BlendState(BlendState::BLEND_ZERO, BlendState::BLEND_ZERO));
		g->drawRectangle(0.0f, 0.0f, w, h);
		g->setBlendState(BlendState(BlendState::BLEND_SRC_ALPHA, BlendState::BLEND_ONE_MINUS_SRC_ALPHA, BlendState::BLEND_ONE, BlendState::BLEND_ONE_MINUS_SRC_ALPHA));

		// Render input rectangle
		g->drawRectangle(0.0f, 0.0f, w, h, Color(0, 0, 0, 127));

		// Get the string to render and set text color
		string text = state->text;

		// Find the visible portion of the text
		int begin = getTextIndexAtPosition(text, rect.position);
		int end = getTextIndexAtPosition(text, rect.position + rect.size);
		string visibleText = text.substr(begin, end - begin);
		float dx = m_font->getStringWidth(text.substr(0, end)) - m_font->getStringWidth(visibleText);

		// Draw and clip the text using scissoring rectangle
		g->enableScissor(0, 0, w, h);
		m_spriteBatch.begin(SpriteBatch::State(SpriteBatch::DEFERRED, BlendState(BlendState::BLEND_SRC_ALPHA, BlendState::BLEND_ONE_MINUS_SRC_ALPHA, BlendState::BLEND_ONE, BlendState::BLEND_ONE_MINUS_SRC_ALPHA)));
		m_font->draw(&m_spriteBatch, textOffset.x + dx, textOffset.y, visibleText);
		m_spriteBatch.end();
		g->disableScissor();

		// Reset render target
		g->setRenderTarget(0);
		 
		// No longer dirty
		m_dirty = false;
	}

	// Draw line edit visualization
	g->setBlendState(BlendState(BlendState::BLEND_ONE, BlendState::BLEND_ONE_MINUS_SRC_ALPHA));
	g->setTexture(m_renderTarget->getTexture());
	g->drawRectangle(rect);
	g->setTexture(0);
	g->setBlendState(BlendState(BlendState::PRESET_ALPHA_BLEND));

	// Draw text cursor
	if(isFocused() && m_cursorTime >= 0.5f)
	{
		g->drawRectangle(
			rect.position.x + textOffset.x + m_font->getStringWidth(state->text.substr(0, state->cursor.getPosition())),
			rect.position.y + textOffset.y,
			2, m_font->getHeight(),
			Color(0, 0, 0, 255)
			);
	}

	// Draw selection rectangle
	g->enableScissor(rect.position.x + 8, g->getHeight() - rect.position.y - rect.size.y, rect.size.x - 16, rect.size.y);
	g->drawRectangle(
		rect.position.x + textOffset.x + m_font->getStringWidth(state->text.substr(0, state->cursor.getSelectionStart())),
		rect.position.y + textOffset.y,
		m_font->getStringWidth(state->text.substr(state->cursor.getSelectionStart(), state->cursor.getSelectionLength())),
		m_font->getHeight(),
		isFocused() ? Color(0, 0, 0, 127) : Color(127, 127, 127, 127)
		);
	g->disableScissor();

	UiObject::onDraw(e);
}

void LineEdit::onResize(ResizeEvent *e)
{
	delete m_renderTarget;
	m_renderTarget = new RenderTarget2D(e->getWidth(), e->getHeight());
	m_dirty = true;
}

void LineEdit::onFocus(FocusEvent * e)
{
	// Mark as dirty
	m_dirty = true;
}

LineEdit::TextState *LineEdit::insertAt(const int pos, const string &str)
{
	TextState *state = *m_undoItr;

	// Add to undo stack
	if(m_textTimer.stop() >= 1.0f)
	{
		state = addUndoState();
	}
	m_textTimer.start();

	// Insert string at index
	string endStr = state->text.substr(pos);
	state->text = state->text.substr(0, pos);
	state->text += str + endStr;

	// Mark as dirty
	m_dirty = true;

	return state;
}

LineEdit::TextState *LineEdit::removeAt(const int pos, const int length)
{
	TextState *state = *m_undoItr;

	// Add to undo stack
	if(m_textTimer.stop() >= 1.0f)
	{
		state = addUndoState();
	}
	m_textTimer.start();

	// Remove char at index
	string endStr = state->text.substr(pos + length);
	state->text = state->text.substr(0, pos);
	state->text += endStr;

	// Mark as dirty
	m_dirty = true;

	return state;
}

int LineEdit::getTextIndexAtPosition(const string &str, Vector2I pos)
{
	// TODO: Use binary search instead of linear
	RectI rect = getDrawRect();
	pos -= rect.position;
	pos -= Vector2F(8.0f - m_offsetX, rect.size.y * 0.5f - m_font->getHeight() * 0.5f);
	for(int i = 0; i < (int) str.size(); ++i)
	{
		if(pos.x < m_font->getStringWidth(str.substr(0, i)))
		{
			return i;
		}
	}
	return str.size();
}

void LineEdit::updateOffset()
{
	Vector2I size = getDrawSize();
	TextState *state = *m_undoItr;
	float cursorPos = m_font->getStringWidth(state->text.substr(0, state->cursor.getPosition()));
	if(cursorPos - m_offsetX > size.x - 16.0f)
	{
		m_offsetX = max(cursorPos - (size.x - 16.0f), 0.0f);
	}
	else if(cursorPos - m_offsetX < 0.0f)
	{
		m_offsetX = cursorPos;
	}
}

LineEdit::TextState *LineEdit::addUndoState()
{
	TextState *state = *m_undoItr;

	m_states.erase(++m_undoItr, m_states.end());

	TextState *newState = new TextState { state->cursor, state->text };
	m_states.push_back(newState);
	m_undoItr = m_states.end();
	m_undoItr--;
	return newState;
}

bool isInvalidChar(char c)
{
	// This removes most control characters
	// including new line
	return c < 32;
}

void LineEdit::onTextInput(TextEvent *e)
{
	// Only add text if active
	if(!isFocused()) return;

	TextState *state = *m_undoItr;

	if(state->cursor.getSelectionLength() > 0)
	{
		// Remove selected text
		state = removeAt(state->cursor.getSelectionStart(), state->cursor.getSelectionLength());
		state->cursor.setPosition(state->cursor.getSelectionStart());
	}

	// Add text
	state = insertAt(state->cursor.getPosition(), string("") + e->getChar());
	state->cursor.moveCursor(1);
}

void LineEdit::onKeyEvent(KeyEvent *e)
{
	// Only add text if active
	if(!isFocused() || e->getType() == KeyEvent::UP) return;

	TextState *state = *m_undoItr;

	const int cursorPosition = state->cursor.getPosition();
	const int selectionLength = state->cursor.getSelectionLength();
	const int selectionPosition = state->cursor.getSelectionStart();
	const bool modShift = (e->getModifiers() & KeyEvent::SHIFT) != 0;
	const bool modCtrl = (e->getModifiers() & KeyEvent::CTRL) != 0;

	switch(e->getKeycode())
	{
		// Enter
		case SAUCE_KEY_RETURN:
		{
			// Call accept function
			if(m_acceptFunc)
			{
				m_acceptFunc();
			}
			break;
		}

		// Backspace
		case SAUCE_KEY_BACKSPACE:
		{
			if(!modCtrl)
			{
				if(selectionLength == 0)
				{
					// Remove char behind
					if(cursorPosition != 0)
					{
						state = removeAt(cursorPosition - 1);
						state->cursor.moveCursor(-1);
					}
				}
				else
				{
					state = removeAt(selectionPosition, selectionLength);
					state->cursor.setPosition(selectionPosition);
				}
			}
			else
			{
				if(cursorPosition != 0)
				{
					// Remove previous word
					int tmp = cursorPosition;
					while(tmp > 0 && state->text[tmp - 1] == ' ') tmp--;
					while(tmp > 0 && state->text[tmp - 1] != ' ') tmp--;
					state = removeAt(tmp, cursorPosition - tmp);
					state->cursor.setPosition(tmp);
				}
			}
			break;
		}

		// Delete
		case SAUCE_KEY_DELETE:
		{
			if(!modCtrl)
			{
				if(selectionLength == 0)
				{
					// Remove char in front
					if(cursorPosition < (int) state->text.size())
					{
						state = removeAt(cursorPosition);
					}
				}
				else
				{
					state = removeAt(selectionPosition, selectionLength);
					state->cursor.setPosition(selectionPosition);
				}
			}
			else
			{
				if(cursorPosition < (int) state->text.size())
				{
					// Remove next word
					int tmp = cursorPosition;
					while(tmp < state->text.size() - 1 && state->text[tmp] != ' ') tmp++;
					while(tmp < state->text.size() - 1 && state->text[tmp + 1] == ' ') tmp++;
					state = removeAt(cursorPosition, tmp - cursorPosition + 1);
					state->cursor.setPosition(cursorPosition);
				}
			}
		}
		break;

		// Left cursor key
		case SAUCE_KEY_LEFT:
		{
			if(modCtrl)
			{
				// Move cursor to the left of the previous word
				int tmp = cursorPosition;
				while(tmp > 0 && state->text[--tmp] == ' ');
				while(tmp > 0 && state->text[tmp - 1] != ' ') tmp--;
				state->cursor.setPosition(tmp, modShift);
			}
			else
			{
				// Move cursor one step to the left
				state->cursor.moveCursor(-1, modShift);
			}
		}
		break;

		// Right cursor key
		case SAUCE_KEY_RIGHT:
		{
			if(modCtrl)
			{
				// Move cursor to the left of the next word
				int tmp = cursorPosition;
				while((uint) tmp < state->text.size() && state->text[++tmp] != ' ');
				while((uint) tmp < state->text.size() && state->text[tmp] == ' ') tmp++;
				state->cursor.setPosition(tmp, modShift);
			}
			else
			{
				// Move cursor one step to the right
				state->cursor.moveCursor(1, modShift);
			}
		}
		break;

		// Home key
		case SAUCE_KEY_HOME:
		{
			state->cursor.setPosition(0, modShift);
		}
		break;

		// End key
		case SAUCE_KEY_END:
		{
			state->cursor.setPosition(state->text.size(), modShift);
		}
		break;

		case SAUCE_KEY_A:
		{
			if(modCtrl)
			{
				state->cursor.setPosition(0);
				state->cursor.setPosition(state->text.size(), true);
			}
		}
		break;

		// Cut and copy
		case SAUCE_KEY_X:
		case SAUCE_KEY_C:
		{
			if(modCtrl)
			{
				if(state->cursor.getSelectionLength() > 0)
				{
					// Put selected text on clipboard
					e->getInputManager()->setClipboardString(state->text.substr(state->cursor.getSelectionStart(), state->cursor.getSelectionLength()));

					if(e->getKeycode() == SAUCE_KEY_X)
					{
						// For cut we remove the selected text afterwards
						state = removeAt(state->cursor.getSelectionStart(), state->cursor.getSelectionLength());
						state->cursor.setPosition(state->cursor.getSelectionStart());
					}
				}
			}
		}
		break;

		// Paste
		case SAUCE_KEY_V:
		{
			if(modCtrl)
			{
				string str = e->getInputManager()->getClipboardString();
				if(!str.empty())
				{
					if(state->cursor.getSelectionLength() > 0)
					{
						// Remove selected text
						state = removeAt(state->cursor.getSelectionStart(), state->cursor.getSelectionLength());
						state->cursor.setPosition(state->cursor.getSelectionStart());
					}

					// Remove all new lines
					str.erase(std::remove_if(str.begin(), str.end(), isInvalidChar), str.end());

					// Insert text
					state = insertAt(state->cursor.getPosition(), str);
					state->cursor.moveCursor(str.size());
				}
			}
		}
		break;

		// Undo
		case SAUCE_KEY_Z:
		{
			if(modShift)
			{
				goto redo;
			}

			if(modCtrl)
			{
				if(!m_states.empty() && m_undoItr != m_states.begin())
				{
					m_undoItr--;
					m_dirty = true;
				}
			}
		}
		break;

		// Redo
		case SAUCE_KEY_Y:
		{
			redo:
			if(modCtrl)
			{
				if(!m_states.empty() && *m_undoItr != m_states.back())
				{
					m_undoItr++;
					m_dirty = true;
				}
			}
		}
		break;
	}

	m_cursorTime = 1.0f;
}

void LineEdit::onClick(ClickEvent *e)
{
	TextState *state = *m_undoItr;
	const Vector2I mousePosition = e->getMouseEvent()->getPosition();
	switch(e->getType())
	{
		case ClickEvent::BEGIN:
		{
			if((e->getClickCount() - 1) % 2 == 0)
			{
				state->cursor.setPosition(getTextIndexAtPosition(state->text, mousePosition));
			}
			else if((e->getClickCount() - 1) % 2 == 1)
			{
				int tmp = getTextIndexAtPosition(state->text, mousePosition);
				while(tmp > 0 && state->text[tmp - 1] == ' ') tmp--;
				while(tmp > 0 && state->text[tmp - 1] != ' ') tmp--;
				state->cursor.setPosition(m_wordBegin = tmp);
				while(tmp < state->text.size() && state->text[tmp++] != ' ');
				while(tmp < state->text.size() && state->text[tmp] == ' ') tmp++;
				state->cursor.setPosition(m_wordEnd = tmp, true);
			}
		}
		break;

		case ClickEvent::DRAG:
		{
			if((e->getClickCount() - 1) % 2 == 0)
			{
				state->cursor.setPosition(getTextIndexAtPosition(state->text, mousePosition), true);
			}
			else if((e->getClickCount() - 1) % 2 == 1)
			{
				int tmp = getTextIndexAtPosition(state->text, mousePosition);
				if(tmp < m_wordBegin)
				{
					state->cursor.setPosition(m_wordEnd);
				}
				else
				{
					state->cursor.setPosition(m_wordBegin);
				}

				if(tmp < m_wordBegin)
				{
					while(tmp > 0 && state->text[--tmp] != ' ');
				}

				if(tmp > 0)
				{
					while(tmp < state->text.size() && state->text[tmp] != ' ') tmp++;
					while(tmp < state->text.size() && state->text[++tmp] == ' ');
				}
				state->cursor.setPosition(tmp, true);
			}
		}
		break;
	}
	m_cursorTime = 1.0f;
}
