#include "UiObject.h"

#include "Game/Scene.h"

#include "Constants.h"

UiObject::UiObject(Scene *scene, UiObject *parent) :
	m_scene(scene),
	m_parent(parent),
	m_anchor(0.0f, 0.0f),
	m_rect(),
	m_hovered(false),
	m_pressed(false),
	m_active(false)
{
	m_scene->addUiObject(this);
}

UiObject::~UiObject()
{
	m_scene->removeUiObject(this);
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
	
void UiObject::update(const float)
{
	if(getRect().contains(Input::getPosition())) {
		m_hovered = true;
		hover();
	}
	else {
		m_hovered = false;
		unhover();
	}
	
	if(m_pressed) {
		if(!Input::isKeyPressed(XD_MOUSE_BUTTON_LEFT)) {
			if(m_hovered) {
				m_active = true;
				clickEvent();
				activateEvent();
			}
			m_pressed = false;
			releaseEvent();
		}
	}
	else {
		if(Input::isKeyPressed(XD_MOUSE_BUTTON_LEFT)) {
			if(m_active && !m_hovered) {
				m_active = false;
				deactivateEvent();
			}
			if(m_hovered) {
				m_pressed = true;
				pressEvent();
			}
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
	Vector2 parentSize = m_parent ? m_parent->getSize() : Window::getSize();
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
	if(!m_parent) return Vector2(Window::getSize()) * m_rect.size;
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