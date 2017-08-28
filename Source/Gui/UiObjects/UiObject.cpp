#include "UiObject.h"

UiObject::UiObject(UiObject *parent, const bool relative) :
	m_parent(parent),
	m_anchor(0.0f, 0.0f),
	m_origin(0.0f, 0.0f),
	m_rect(0, 0, 0, 0),
	m_drawRect(0, 0, 0, 0),
	m_relative(relative),
	m_hovered(false),
	m_pressed(false),
	m_focused(false),
	m_beingResized(false),
	m_clickCount(0),
	m_clickTimer()
{
	if(parent)
	{
		parent->addChildLast(this);
	}
}

UiObject::~UiObject()
{
}

bool UiObject::isFocused() const
{
	return m_focused;
}

void UiObject::setFocused(const bool focused)
{
	m_focused = focused;
}

bool UiObject::isHovered() const
{
	return m_hovered;
}

bool UiObject::isPressed() const
{
	return m_pressed;
}

void UiObject::onResize(ResizeEvent *e)
{
	for(SceneObject *child : getChildren())
	{
		UiObject *uiChild = dynamic_cast<UiObject*>(child);
		if(uiChild)
		{
			uiChild->updateDrawRect();
			ResizeEvent e(uiChild);
			uiChild->onResize(&e);
		}
	}
}

void UiObject::setPosition(const Vector2F &position)
{
	setPosition(position.x, position.y);
}

void UiObject::setPosition(const float x, const float y)
{
	if(m_relative)
	{
		m_rect.position.set(x, y);
		updateDrawRect();
	}
	else
	{
		m_drawRect.position.set(x, y);
	}
}

Vector2F UiObject::getPosition() const
{
	return m_relative ? m_rect.position : m_drawRect.position;
}

void UiObject::setOrigin(const Vector2F &origin)
{
	setOrigin(origin.x, origin.y);
}

void UiObject::setOrigin(const float x, const float y)
{
	m_origin.set(x, y);
	updateDrawRect();
}

Vector2F UiObject::getOrigin() const
{
	return m_origin;
}

void UiObject::setSize(const Vector2F &size)
{
	setSize(size.x, size.y);
}

void UiObject::setSize(const float width, const float height)
{
	if(m_relative)
	{
		m_rect.size.set(width, height);
		updateDrawRect();
	}
	else
	{
		m_drawRect.size.set(width, height);
	}

	if(!m_beingResized)
	{
		// Call resize on this object and its children
		ResizeEvent e(this);
		onResize(&e);
	}
}

Vector2F UiObject::getSize() const
{
	return m_relative ? m_rect.size : m_drawRect.size;
}

void UiObject::setRect(const RectF &rect)
{
	if(m_relative)
	{
		m_rect = rect;
		updateDrawRect();
	}
	else
	{
		m_drawRect = rect;
	}
}

RectF UiObject::getRect() const
{
	return m_relative ? m_rect : m_drawRect;
}

float UiObject::getAspectRatio() const
{
	const Vector2I drawSize = getDrawSize();
	return (float) drawSize.x / (float) drawSize.y;
}

Vector2F UiObject::getAnchor() const
{
	return m_anchor;
}

void UiObject::setAnchor(const Vector2F &anchor)
{
	setAnchor(anchor.x, anchor.y);
}

void UiObject::setAnchor(const float x, const float y)
{
	m_anchor.set(x, y);
	updateDrawRect();
}

Vector2I UiObject::getDrawPosition() const
{
	return m_drawRect.position;
}

Vector2I UiObject::getDrawSize() const
{
	return m_drawRect.size;
}

RectI UiObject::getDrawRect() const
{
	return m_drawRect;
}

void UiObject::onMouseEvent(MouseEvent *e)
{
	switch(e->getType())
	{
		case MouseEvent::MOVE:
		{
			if(getDrawRect().contains(e->getPosition()))
			{
				m_hovered = true;
				HoverEvent event(HoverEvent::ENTER, e);
				onHover(&event);
			}
			else
			{
				m_hovered = false;
				HoverEvent event(HoverEvent::LEAVE, e);
				onHover(&event);
			}

			if(m_pressed)
			{
				ClickEvent event(ClickEvent::DRAG, m_clickCount, e);
				onClick(&event);
			}
		}
		break;

		case MouseEvent::DOWN:
		{
			if(m_hovered)
			{
				m_pressed = true;

				if(m_clickTimer.stop() < 0.2f)
				{
					m_clickTimer.start();
				}
				else
				{
					m_clickCount = 0;
				}

				m_clickCount++;

				ClickEvent event(ClickEvent::BEGIN, m_clickCount, e);
				onClick(&event);
			}
		}
		break;

		case MouseEvent::UP:
		{
			if(m_pressed)
			{
				if(m_hovered)
				{
					ClickEvent event(ClickEvent::DONE, m_clickCount, e);
					onClick(&event);

					if(m_clickTimer.stop() < 0.2f)
					{
						m_clickTimer.start();
					}
					else
					{
						m_clickCount = 0;
					}

					if(!m_focused)
					{
						m_focused = true;
						FocusEvent event(FocusEvent::GAINED);
						onFocus(&event);
					}
				}
				else
				{
					ClickEvent event(ClickEvent::CANCELED, m_clickCount, e);
					onClick(&event);
				}
				m_pressed = false;
			}
			else if(!m_hovered && m_focused)
			{
				m_focused = false;
				FocusEvent event(FocusEvent::LOST);
				onFocus(&event);
			}
		}
		break;
	}

	SceneObject::onMouseEvent(e);
}


void UiObject::updateDrawRect()
{
	m_drawRect.position = (m_parent->m_drawRect.position + m_parent->m_drawRect.size * m_anchor) + (getPosition() - getSize() * m_origin) * m_parent->m_drawRect.size;
	m_drawRect.size = getSize() * m_parent->m_drawRect.size;
}

ResizeEvent::ResizeEvent(UiObject *uiObject) :
	Event(EVENT_GUI_RESIZE),
	m_uiObject(uiObject),
	m_width(uiObject->getDrawRect().size.x),
	m_height(uiObject->getDrawRect().size.y)
{
	m_uiObject->m_beingResized = true;
}

ResizeEvent::~ResizeEvent()
{
	m_uiObject->m_beingResized = false;
}