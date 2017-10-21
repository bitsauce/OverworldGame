#include "LineEdit.h"

LineEdit::LineEdit(UiObject *parent, const uint width, const uint height,
				   Resource<Font> font,
				   Resource<Texture2D> activeTexture,
				   Resource<Texture2D> inactiveTexture,
				   const uint borderSize) :
	UiObject(parent),
	m_cursorTime(0.0f),
	m_offsetX(0.0f),
	m_font(font),
	m_textureActive(activeTexture),
	m_textureInactive(inactiveTexture),
	m_renderTarget(0),
	m_dirtyGraphics(true),
	m_dirtyTextGraphics(true),
	m_spriteBatch(100),
	m_wordBegin(0),
	m_wordEnd(0),
	m_defaultText(""),
	m_color(0, 0, 0, 255),
	m_renderTargetText(0),
	m_borderSize(borderSize)
{
	setText("");
	m_renderTarget = new RenderTarget2D(width, height);
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

	m_dirtyTextGraphics = true;
}

string LineEdit::getText() const
{
	return (*m_undoItr)->text;
}

void LineEdit::setTextColor(const Color &color)
{
	m_color = color;
}

void LineEdit::setDefaultText(const string &def)
{
	m_defaultText = def;
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
	RectF rect = getDrawRect();
	GraphicsContext *graphicsContext = e->getGraphicsContext();

	// Update background graphics if dirty
	if(m_dirtyGraphics)
	{
		// Update line edit visualization
		graphicsContext->pushState();
		graphicsContext->setRenderTarget(m_renderTarget);
		graphicsContext->setBlendState(BlendState(BlendState::BLEND_ONE, BlendState::BLEND_ZERO, BlendState::BLEND_ONE, BlendState::BLEND_ZERO));

		// Set texture
		if(isFocused())
		{
			graphicsContext->setTexture(m_textureActive);
		}
		else
		{
			graphicsContext->setTexture(m_textureInactive);
		}

		// Render input rectangle
		const float w = m_renderTarget->getWidth(), h = m_renderTarget->getHeight();
		graphicsContext->drawRectangle(0.0f, 0.0f, 16.0f, 16.0f, Color::White, TextureRegion(0.0f, 0.0f, 1.0f / 3.0f, 1.0f / 3.0f));
		graphicsContext->drawRectangle(w - 16.0f, 0.0f, 16.0f, 16.0f, Color::White, TextureRegion(2.0f / 3.0f, 0.0f, 1.0f, 1.0f / 3.0f));
		graphicsContext->drawRectangle(0.0f, h - 16.0f, 16.0f, 16.0f, Color::White, TextureRegion(0.0f, 2.0f / 3.0f, 1.0f / 3.0f, 1.0f));
		graphicsContext->drawRectangle(w - 16.0f, h - 16.0f, 16.0f, 16.0f, Color::White, TextureRegion(2.0f / 3.0f, 2.0f / 3.0f, 1.0f, 1.0f));
		graphicsContext->drawRectangle(16.0f, 0.0f, w - 32.0f, 16.0f, Color::White, TextureRegion(1.0f / 3.0f, 0.0f / 3.0f, 2.0f / 3.0f, 1.0f / 3.0f));
		graphicsContext->drawRectangle(16.0f, h - 16.0f, w - 32.0f, 16.0f, Color::White, TextureRegion(1.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));
		graphicsContext->drawRectangle(0.0f, 16.0f, 16.0f, h - 32.0f, Color::White, TextureRegion(0.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f));
		graphicsContext->drawRectangle(w - 16.0f, 16.0f, 16.0f, h - 32.0f, Color::White, TextureRegion(2.0f / 3.0f, 1.0f / 3.0f, 3.0f / 3.0f, 2.0f / 3.0f));
		graphicsContext->drawRectangle(16.0f, 16.0f, w - 32.0f, h - 32.0f, Color::White, TextureRegion(1.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f));
		graphicsContext->popState();

		m_dirtyGraphics = false;
	}

	// Update text offset if dirty
	if(m_dirtyTextGraphics)
	{
		updateOffset();
	}

	// Get text offset
	Vector2I textOffset(8.0f - m_offsetX, rect.size.y * 0.5f - m_font->getHeight() * 0.5f);

	// Update text graphics if dirty
	if(m_dirtyTextGraphics)
	{
		graphicsContext->pushState();
		graphicsContext->setRenderTarget(m_renderTargetText);

		// Clear previous text
		// OPTIMIZATION: Only update the text which changed
		graphicsContext->setBlendState(BlendState(BlendState::BLEND_ZERO, BlendState::BLEND_ZERO));
		graphicsContext->drawRectangle(0, 0, m_renderTargetText->getWidth(), m_renderTargetText->getHeight());

		// Get the string to render and set text color
		string text;
		if(state->text.empty())
		{
			// Use default string if line edit is empty
			text = m_defaultText;
			m_font->setColor(Color(127, 127, 127, 255));
		}
		else
		{
			text = state->text;
			m_font->setColor(m_color);
		}

		// Find the visible portion of the text
		int begin = getTextIndexAtPosition(text, rect.position);
		int end = getTextIndexAtPosition(text, rect.position + rect.size);
		string visibleText = text.substr(begin, end - begin);
		float dx = m_font->getStringWidth(text.substr(0, end)) - m_font->getStringWidth(visibleText);

		// Draw and clip the text using scissoring rectangle
		const float w = m_renderTargetText->getWidth(), h = m_renderTargetText->getHeight();
		graphicsContext->enableScissor(m_borderSize, 0, w - m_borderSize * 2, h);
		m_spriteBatch.begin(graphicsContext, SpriteBatch::State(SpriteBatch::DEFERRED, BlendState(BlendState::BLEND_SRC_ALPHA, BlendState::BLEND_ONE_MINUS_SRC_ALPHA, BlendState::BLEND_ONE, BlendState::BLEND_ONE_MINUS_SRC_ALPHA)));
		m_font->draw(&m_spriteBatch, textOffset.x + dx, textOffset.y, visibleText);
		m_spriteBatch.end();
		graphicsContext->disableScissor();

		// Reset render target
		graphicsContext->popState();

		// No longer dirty
		m_dirtyTextGraphics = false;
	}

	// Draw line edit visualization
	graphicsContext->setTexture(m_renderTarget->getTexture());
	graphicsContext->drawRectangle(rect);
	graphicsContext->setTexture(m_renderTargetText->getTexture());
	graphicsContext->drawRectangle(rect);
	graphicsContext->setTexture(0);

	// Draw text cursor
	if(isFocused() && m_cursorTime >= 0.5f)
	{
		graphicsContext->drawRectangle(
			rect.position.x + textOffset.x + m_font->getStringWidth(state->text.substr(0, state->cursor.getPosition())),
			rect.position.y + textOffset.y,
			2, m_font->getHeight(),
			m_color
			);
	}

	// Draw selection rectangle
	graphicsContext->enableScissor(rect.position.x + 8, graphicsContext->getHeight() - rect.position.y - rect.size.y, rect.size.x - 16, rect.size.y);
	graphicsContext->drawRectangle(
		rect.position.x + textOffset.x + m_font->getStringWidth(state->text.substr(0, state->cursor.getSelectionStart())),
		rect.position.y + textOffset.y,
		m_font->getStringWidth(state->text.substr(state->cursor.getSelectionStart(), state->cursor.getSelectionLength())),
		m_font->getHeight(),
		isFocused() ? Color(0, 0, 0, 127) : Color(127, 127, 127, 127)
		);
	graphicsContext->disableScissor();

	UiObject::onDraw(e);
}

void LineEdit::onResize(ResizeEvent *e)
{
	delete m_renderTargetText;
	m_renderTargetText = new RenderTarget2D(e->getWidth(), e->getHeight());
	m_dirtyTextGraphics = true;
}

void LineEdit::onFocus(FocusEvent * e)
{
	// Mark as dirty
	m_dirtyGraphics = true;
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
	m_dirtyTextGraphics = true;

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
	m_dirtyTextGraphics = true;

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

			if(modCtrl)
			{
				if(!m_states.empty() && m_undoItr != m_states.begin())
				{
					m_undoItr--;
					m_dirtyTextGraphics = true;
				}
			}

			else if(!modShift)
			{
				break;
			}
		}

		// Redo
		case SAUCE_KEY_Y:
		{
			if(modCtrl)
			{
				if(!m_states.empty() && *m_undoItr != m_states.back())
				{
					m_undoItr++;
					m_dirtyTextGraphics = true;
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
