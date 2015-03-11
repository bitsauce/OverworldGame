#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include "Config.h"

#pragma warning(disable : 4717)

class UiObject
{
public:
	UiObject(UiObject *parent);
	virtual ~UiObject();

	// Mouse press event
	virtual void pressEvent() { }
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
	void setPosition(const Vector2 &position);
	Vector2 getPosition() const;

	// Set/get size
	void setSize(const Vector2 &size);
	Vector2 getSize() const;

	// Get rectangle
	Rect getRect() const;

	// Set anchor
	void setAnchor(const Vector2 &anchor);
	
	// Update & draw
	virtual void update(const float dt);
	virtual void draw(SpriteBatch *SpriteBatch, const float alpha) { }

protected:
	// Parent object
	UiObject *m_parent;

	// Rectangle of the object in relative coordinates [0-1]
	Rect m_rect;
	
	// Screen anchor [0-1]
	Vector2 m_anchor;

	// Object state
	bool m_hovered;
	bool m_pressed;
	bool m_active;
};

#endif // UI_OBJECT_H