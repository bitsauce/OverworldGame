#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include "Config.h"
#include "Gui/UiEvents.h"

class UiObject : public SceneObject
{
public:
	UiObject(UiObject *parent, const bool relative = true);
	virtual ~UiObject();

	// Mouse hover
	virtual void onHover(HoverEvent*) { }
	bool isHovered() const;

	// Active state
	virtual void onFocus(FocusEvent*) { }
	bool isFocused() const;

	// Set focused
	void setFocused(const bool focused);

	// Click event
	virtual void onClick(ClickEvent*) { }
	bool isPressed() const;

	// Resize event
	virtual void onResize(ResizeEvent *e);

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

	float getAspectRatio() const;

	UiObject *getParent() const { return m_parent; }

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

	Vector2I getDrawPosition() const;
	Vector2I getDrawSize() const;
	RectI getDrawRect() const;

	/**
	 * \fn	virtual void UiObject::onMouseEvent(MouseEvent *e);
	 *
	 * \brief	Override mouse event to handle UI specific functionality.
	 *
	 * \param [in,out]	e	If non-null, the MouseEvent to process.
	 */

	virtual void onMouseEvent(MouseEvent *e);

protected:
	void updateDrawRect();

private:
	/** \brief	The parent UiObject. */
	UiObject *const m_parent;

	/** \brief	Rectangle of the ui element in relative coordinates [0, 1]. */
	RectF m_rect;
	const bool m_relative;

	/** \brief	Rectangle of the ui element in screen coordinates. */
	RectI m_drawRect;

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

	friend class ResizeEvent;
	bool m_beingResized;
};

#endif // UI_OBJECT_H