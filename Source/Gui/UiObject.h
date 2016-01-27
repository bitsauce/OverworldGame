#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include "Config.h"

class UiObject : public SceneObject
{
public:
	UiObject();
	virtual ~UiObject();

	// Mouse press event
	/*virtual void pressEvent() { }
	virtual void releaseEvent() { }
	bool isPressed() const;
	
	// Mouse hover
	virtual void hover() { }
	virtual void unhover() { }
	bool isHovered() const;

	// Active state
	virtual void activateEvent() { }
	virtual void deactivateEvent() { }
	bool isActive() const;

	// Click event
	virtual void clickEvent() { }
	
	// Set/get position
	void setPosition(const Vector2F &position);
	Vector2F getPosition() const;

	// Set/get size
	void setSize(const Vector2F &size);
	Vector2F getSize() const;

	// Get rectangle
	Rect getRect() const;

	// Set anchor
	void setAnchor(const Vector2F &anchor);

	// Make active
	void setActive(const bool active)
	{
		m_active = active;
	}
	
	// Update & draw
	virtual void onTick(TickEvent *e);
	virtual void onDraw(DrawEvent *e) { }
	*/
protected:
	// Scene object
	Scene *m_scene;

	// Parent object
	//UiObject *m_parent;

	// Rectangle of the object in relative coordinates [0-1]
	RectF m_rect;
	
	// Screen anchor [0-1]
	Vector2F m_anchor;

	// Object state
	bool m_hovered;
	bool m_pressed;
	bool m_active;
};

#endif // UI_OBJECT_H