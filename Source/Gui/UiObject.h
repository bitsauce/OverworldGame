#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include "Config.h"
#include "UiEvents.h"

class UiObject : public SceneObject
{
public:
	UiObject(UiObject *parent);
	virtual ~UiObject();

	// Mouse hover
	virtual void onHover(HoverEvent*) { }
	bool isHovered() const;

	// Active state
	virtual void onFocus(FocusEvent*) { }
	bool isFocused() const;

	/**
	 * \fn	void UiObject::setFocused(const bool focused);
	 *
	 * \brief	Make active.
	 *
	 * \param	focused	The focused.
	 */

	void setFocused(const bool focused);

	// Click event
	virtual void onClick(ClickEvent*) { }
	bool isPressed() const;

	// Resize event
	virtual void onResize(ResizeEvent *e)
	{
		for(SceneObject *child : getChildren())
		{
			UiObject *uiChild = dynamic_cast<UiObject*>(child);
			if(uiChild)
			{
				Vector2I size = uiChild->getDrawSize();
				ResizeEvent e(size.x, size.y);
				uiChild->onResize(&e);
			}
		}
	}

	// Set/get position
	void setPosition(const Vector2F &position);
	void setPosition(const float x, const float y);
	Vector2F getPosition() const;

	// Set/get origin
	void setOrigin(const Vector2F &origin);
	void setOrigin(const float x, const float y);
	Vector2F getOrigin() const;

	// Set/get size
	void setSize(const Vector2F &size);
	void setSize(const float width, const float height);
	Vector2F getSize() const;

	void setRect(const RectF &rect);
	RectF getRect() const;

	/**
	 * \fn	void UiObject::setAnchor(const float x, const float y);
	 *
	 * \brief	Set anchoring point.
	 *
	 * \param	x	The x coordinate of anchor.
	 * \param	y	The y coordinate of anchor.
	 */

	void setAnchor(const float x, const float y);
	void setAnchor(const Vector2F &anchor);
	Vector2F getAnchor() const;

	virtual Vector2I getDrawPosition();
	virtual Vector2I getDrawSize();
	RectI getDrawRect();

	virtual void onDraw(DrawEvent *e);

	/**
	 * \fn	virtual void UiObject::onMouseEvent(MouseEvent *e);
	 *
	 * \brief	Override mouse event to handle UI specific functionality.
	 *
	 * \param [in,out]	e	If non-null, the MouseEvent to process.
	 */

	virtual void onMouseEvent(MouseEvent *e);
	
private:
	/** \brief	The parent UiObject. */
	UiObject * const m_parent;

	/** \brief	Rectangle of the ui element in relative coordinates [0, 1]. */
	RectF m_rect;

	/** \brief	The click timer. */
	SimpleTimer m_clickTimer;

	/** \brief	Number of clicks. */
	int m_clickCount;

	/** \brief	Screen anchor in relative coordinates [0, 1]. */
	Vector2F m_anchor;

	/** \brief	The origin. */
	Vector2F m_origin;

	/** \brief	true if cursor is hovering the ui object. */
	bool m_hovered;
	
	/** \brief	true if ui object is pressed. */
	bool m_pressed;

	/** \brief	true if ui object has focus. */
	bool m_focused;
};

#endif // UI_OBJECT_H