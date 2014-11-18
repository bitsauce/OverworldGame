#include "uiobject.h"

UiObject::UiObject(UiObject *parent) :
	m_parent(parent),
	m_anchor(0.0f, 0.0f),
	m_rect(),
	m_hovered(false),
	m_pressed(false),
	m_active(false)
{
}
	
bool UiObject::isPressed() const
{
	return m_pressed;
}

bool UiObject::isHovered() const
{
	return m_hovered;
}

bool UiObject::isActive() const
{
	return m_active;
}
	
void UiObject::update()
{
	if(getRect().contains(XInput::getPosition())) {
		m_hovered = true;
		hover();
	}
	else {
		m_hovered = false;
		unhover();
	}
	
	if(m_pressed) {
		if(!XInput::getKeyState(XD_LMB)) {
			if(isHovered()) {
				m_active = true;
				clickEvent();
				activateEvent();
			}
			else {
				m_active = false;
				deactivateEvent();
			}
			m_pressed = false;
			releaseEvent();
		}
	}
	else {
		if(m_hovered && XInput::getKeyState(XD_LMB)) {
			m_pressed = true;
			pressEvent();
		}
	}
}

void UiObject::setPosition(const Vector2 &position)
{
	m_rect.position = position;
}

Vector2 UiObject::getPosition() const
{
	Vector2 parentPos = m_parent ? m_parent->getPosition() : m_rect.position;
	Vector2 parentSize = m_parent ? m_parent->getSize() : XWindow::getSize();
	Vector2 pos = m_rect.position;
	Vector2 size = m_rect.size;
			
	parentPos += parentSize * m_anchor;
	pos -= size * m_anchor;
	
	return parentPos + pos * parentSize;
}

void UiObject::setSize(const Vector2 &size)
{
	m_rect.size = size;
}

Vector2 UiObject::getSize() const
{
	if(!m_parent) return Vector2(XWindow::getSize()) * m_rect.size;
	return m_parent->getSize() * m_rect.size;
}
	
Rect UiObject::getRect() const
{
	return Rect(getPosition(), getSize());
}

void UiObject::setAnchor(const Vector2 &anchor)
{
	m_anchor = anchor;
}