#ifndef LINE_EDIT_H
#define LINE_EDIT_H

#include "UiObject.h"

class UiObject;

/**
 * \class	LineEdit
 *
 * \brief	A single line text input.
 */

class LineEdit : public UiObject
{
	friend class Cursor;
public:
	LineEdit(UiObject *parent, const uint width, const uint height);
	~LineEdit();

	/**
	 * \fn	void LineEdit::setText(const string &text);
	 *
	 * \brief	Sets the input string in the line edit.
	 *
	 * \param	text	The input string.
	 */

	void setText(const string &text);

	/**
	 * \fn	string LineEdit::getText() const;
	 *
	 * \brief	Gets the input string in the line edit.
	 *
	 * \return	The input string.
	 */

	string getText() const;

	/**
	 * \fn	void LineEdit::setAcceptFunc(function<void()> func);
	 *
	 * \brief	Sets the function called when return is pressed.
	 *
	 * \param	func	The function pointer.
	 */

	void setAcceptFunc(function<void()> func);

	/**
	 * \fn	void LineEdit::setTextColor(const Color &color);
	 *
	 * \brief	Sets text color.
	 *
	 * \param	color	The text color.
	 */

	void setTextColor(const Color &color);

	/**
	 * \fn	void LineEdit::setDefaultText(const string &def);
	 *
	 * \brief	Sets the string which is show when the line edit is empty.
	 *
	 * \param	def	The default string.
	 */

	void setDefaultText(const string &def);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	void onResize(ResizeEvent *e);
	void onFocus(FocusEvent *e);

	class Cursor
	{
	public:
		Cursor(LineEdit *lineEdit) :
			m_lineEdit(lineEdit),
			m_anchorPosition(0),
			m_position(0),
			m_length(0)
		{
		}

		void moveCursor(const int dt, const bool anchor = false)
		{
			setPosition(m_position + dt, anchor);
		}

		void setPosition(const int position, const bool anchor = false)
		{
			// If no anchor is set, set anchor
			if(anchor && m_length == 0)
			{
				m_anchorPosition = m_position;
			}
			
			// Update position
			m_position = math::clamp(position, 0, m_lineEdit->getText().size());

			// Calculate selection length
			if(anchor)
			{
				m_length = math::abs(position - m_anchorPosition);
			}
			else
			{
				m_length = 0;
			}

			// Update text offsets
			m_lineEdit->m_dirtyTextGraphics = true;
		}

		void setLength(const int length)
		{
			m_length = length;
		}

		int getPosition() const
		{
			return m_position;
		}

		int getSelectionLength() const
		{
			return m_length;
		}

		int getSelectionStart() const
		{
			return min(m_position, m_anchorPosition);
		}

	private:
		LineEdit *m_lineEdit;
		int m_anchorPosition;
		int m_position;
		int m_length;
	};

	struct TextState
	{
		Cursor cursor;
		string text;
	};

protected:
	TextState *insertAt(const int pos, const string &str);
	TextState *removeAt(const int pos, const int length = 1);
	TextState *addUndoState();
	int getTextIndexAtPosition(const string &str, Vector2I pos);
	void updateOffset();
	void onTextInput(TextEvent *e);
	void onKeyEvent(KeyEvent *e);
	void onClick(ClickEvent *e);

	// Visualization
	SpriteBatch m_spriteBatch;
	Resource<Texture2D> m_textureActive, m_textureInactive;
	RenderTarget2D *m_renderTarget;
	RenderTarget2D *m_renderTargetText;
	Resource<Font> m_font;
	Color m_color;

	// Data
	SimpleTimer m_textTimer;
	list<TextState*> m_states;
	list<TextState*>::iterator m_undoItr;
	string m_defaultText;
	int m_wordBegin, m_wordEnd;
	float m_cursorTime;
	float m_offsetX;
	bool m_dirtyGraphics, m_dirtyTextGraphics;
	function<void()> m_acceptFunc;
};

#endif // LINE_EDIT_H